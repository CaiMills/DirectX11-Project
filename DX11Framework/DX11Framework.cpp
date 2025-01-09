#include "DX11Framework.h"
#include <atlstr.h> // to use CString.
#include "Geometry.h"

#define FPS60 1.0f/60.0f

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

HRESULT DX11Framework::Initialise(HINSTANCE hInstance, int nShowCmd)
{
    HRESULT hr = S_OK;

    _timer = new Timer();

    hr = CreateWindowHandle(hInstance, nShowCmd);
    if (FAILED(hr)) return E_FAIL;

    hr = CreateD3DDevice();
    if (FAILED(hr)) return E_FAIL;

    hr = CreateSwapChainAndFrameBuffer();
    if (FAILED(hr)) return E_FAIL;

    hr = InitShadersAndInputLayout();
    if (FAILED(hr)) return E_FAIL;

    hr = InitPipelineVariables();
    if (FAILED(hr)) return E_FAIL;

    hr = InitRunTimeData();
    if (FAILED(hr)) return E_FAIL;

    return hr;
}

HRESULT DX11Framework::CreateWindowHandle(HINSTANCE hInstance, int nCmdShow)
{
    const wchar_t* windowName  = L"Direct X11 Project";

    WNDCLASSW wndClass;
    wndClass.style = 0;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = 0;
    wndClass.hIcon = 0;
    wndClass.hCursor = 0;
    wndClass.hbrBackground = 0;
    wndClass.lpszMenuName = 0;
    wndClass.lpszClassName = windowName;

    RegisterClassW(&wndClass);

    _windowHandle = CreateWindowExW(0, windowName, windowName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 
        _WindowWidth, _WindowHeight, nullptr, nullptr, hInstance, nullptr);

    return S_OK;
}

HRESULT DX11Framework::CreateD3DDevice()
{
    HRESULT hr = S_OK;

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
    };

    ID3D11Device* baseDevice;
    ID3D11DeviceContext* baseDeviceContext;

    DWORD createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT | createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &baseDevice, nullptr, &baseDeviceContext);
    if (FAILED(hr))
    {
        return hr;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    hr = baseDevice->QueryInterface(__uuidof(ID3D11Device), reinterpret_cast<void**>(&_device));
    hr = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext), reinterpret_cast<void**>(&_immediateContext));

    baseDevice->Release();
    baseDeviceContext->Release();

    ///////////////////////////////////////////////////////////////////////////////////////////////

    hr = _device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&_dxgiDevice));
    if (FAILED(hr))
    {
        return hr;
    }

    IDXGIAdapter* dxgiAdapter;
    hr = _dxgiDevice->GetAdapter(&dxgiAdapter);
    hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&_dxgiFactory));
    dxgiAdapter->Release();

    return S_OK;
}

HRESULT DX11Framework::CreateSwapChainAndFrameBuffer()
{
    HRESULT hr = S_OK;

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
    swapChainDesc.Width = 0; // Defer to WindowWidth
    swapChainDesc.Height = 0; // Defer to WindowHeight
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //FLIP* modes don't support sRGB back buffer
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = 0;

    hr = _dxgiFactory->CreateSwapChainForHwnd(_device, _windowHandle, &swapChainDesc, nullptr, nullptr, &_swapChain);
    if (FAILED(hr))
    {
        return hr;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3D11Texture2D* frameBuffer = nullptr;

    hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&frameBuffer));
    if (FAILED(hr))
    {
        return hr;
    }

    D3D11_RENDER_TARGET_VIEW_DESC framebufferDesc = {};
    framebufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //sRGB render target enables hardware gamma correction
    framebufferDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    hr = _device->CreateRenderTargetView(frameBuffer, &framebufferDesc, &_frameBufferView);

    D3D11_TEXTURE2D_DESC depthBufferDesc = {};
    frameBuffer->GetDesc(&depthBufferDesc);

    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    _device->CreateTexture2D(&depthBufferDesc, nullptr, &_depthStencilBuffer);
    _device->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

    frameBuffer->Release(); //Release after depth buffer is created

    return hr;
}

HRESULT DX11Framework::InitShadersAndInputLayout()
{
    HRESULT hr = S_OK;
    ID3DBlob* errorBlob;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
    
    ID3DBlob* vsBlob;

    //Skybox Vertex Shader
    hr = D3DCompileFromFile(L"SkyboxShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_main", "vs_5_0", dwShaderFlags, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    hr = _device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &_vertexShaderSkybox);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    //Primary Vertex Shader
    hr =  D3DCompileFromFile(L"SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_main", "vs_5_0", dwShaderFlags, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    hr = _device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &_vertexShader);
    if (FAILED(hr))
    {
        return hr;
    }

    ID3DBlob* psBlob;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    //Skybox Pixel Shader
    hr = D3DCompileFromFile(L"SkyboxShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0", dwShaderFlags, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    hr = _device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &_pixelShaderSkybox);
    if (FAILED(hr))
    {
        return hr;
    }

    //Primary Pixel Shader
    hr = D3DCompileFromFile(L"SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0", dwShaderFlags, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = _device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &_inputLayout);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = _device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &_pixelShader);

    vsBlob->Release();
    psBlob->Release();
    errorBlob->Release();

    return hr;
}

HRESULT DX11Framework::InitPipelineVariables()
{
    HRESULT hr = S_OK;

    //Input Assembler
    _immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _immediateContext->IASetInputLayout(_inputLayout);

    // Rasterizer
    D3D11_RASTERIZER_DESC cmdesc;
    ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
    cmdesc.FillMode = D3D11_FILL_WIREFRAME;
    cmdesc.CullMode = D3D11_CULL_NONE;
    hr = _device->CreateRasterizerState(&cmdesc, &_wireframeState);

    ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
    cmdesc.FillMode = D3D11_FILL_SOLID;
    cmdesc.CullMode = D3D11_CULL_BACK;
    cmdesc.FrontCounterClockwise = true;
    hr = _device->CreateRasterizerState(&cmdesc, &_fillState);

    cmdesc.FrontCounterClockwise = false;
    hr = _device->CreateRasterizerState(&cmdesc, &_fillState);

    _immediateContext->RSSetState(_fillState);

    D3D11_DEPTH_STENCIL_DESC dssDesc;
    ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dssDesc.DepthEnable = true;
    dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    _device->CreateDepthStencilState(&dssDesc, &_DSLessEqual);

    _immediateContext->OMSetDepthStencilState(_DSLessEqual, 0);

    //Bilinear Sampler
    D3D11_SAMPLER_DESC bilinearSampledesc = {};
    bilinearSampledesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    bilinearSampledesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    bilinearSampledesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    bilinearSampledesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    bilinearSampledesc.MaxLOD = D3D11_FLOAT32_MAX;
    bilinearSampledesc.MinLOD = 0;

    hr = _device->CreateSamplerState(&bilinearSampledesc, &_bilinearSamplerState);
    if (FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

HRESULT DX11Framework::InitRunTimeData()
{
    HRESULT hr = S_OK;

    //Constant Buffer
    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = _device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffer);
    if (FAILED(hr))
    {
        return hr;
    }

    _viewport = { 0.0f, 0.0f, (float)_WindowWidth, (float)_WindowHeight, 0.0f, 1.0f };
    _immediateContext->RSSetViewports(1, &_viewport);

    // Setup Camera
    XMFLOAT3 eye = XMFLOAT3(0.0f, 2.0f, -1.0f);
    XMFLOAT3 at = XMFLOAT3(0.0f, 2.0f, 0.0f);
    XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

    _camera = new Camera(eye, at, up, (float)_WindowWidth, (float)_WindowHeight, 0.01f, 200.0f);

    //Lighting
    InitLighting();

    //Initiate Scene
    Geometry geo; //Geometry Reference
    ID3D11ShaderResourceView* _texture;

    //Skybox
    Appearance* _appearance = new Appearance(geo.Cube(_device, true));
    CreateDDSTextureFromFile(_device, L"Textures\\Free Assets Online\\spyro3Skybox.dds", nullptr, &_texture);
    _appearance->SetTexture(_texture);

    D3D11_DEPTH_STENCIL_DESC dsDescSkybox = { };
    dsDescSkybox.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    dsDescSkybox.DepthEnable = true;
    dsDescSkybox.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

    hr = _device->CreateDepthStencilState(&dsDescSkybox, &_depthStencilSkybox);
    if (FAILED(hr))
    {
        return hr;
    }
    _skybox->SetType("Skybox");
    _skybox->SetAppearance(_appearance);

    //Geometry
    _appearance = new Appearance(geo.Plane(_device));
    CreateDDSTextureFromFile(_device, L"Textures\\Test Textures\\floor.dds", nullptr, &_texture);
    _appearance->SetTexture(_texture);

    _floor->SetType("Floor");
    _floor->SetAppearance(_appearance);
    _floor->GetTransform()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    _floor->GetTransform()->SetScale(Vector3(15.0f, 15.0f, 15.0f));
    _floor->GetTransform()->SetRotation(Vector3(XMConvertToRadians(90.0f), 0.0f, 0.0f));

    _gameObjects.push_back(_floor);

    for (auto i = 0; i < 4; i++)
    {
        _appearance = new Appearance(geo.Cube(_device, false));
        CreateDDSTextureFromFile(_device, L"Textures\\Test Textures\\stone.dds", nullptr, &_texture);
        _appearance->SetTexture(_texture);

        _cubes[i].SetType("Cube " + i);
        _cubes[i].SetAppearance(_appearance);
        _cubes[i].GetTransform()->SetPosition(Vector3(-2.0f + (i * 2.5f), 1.0f, 10.0f));
        _cubes[i].GetTransform()->SetScale(Vector3(1.0f, 1.0f, 1.0f));

        _gameObjects.push_back(&_cubes[i]);
    }

    //GameObjects
    InitGameObjects();
}

DX11Framework::~DX11Framework()
{
    if (_camera) { delete _camera; }
    if (_skybox) { delete _skybox; }
    for (auto gameObject : _gameObjects)
    {
        delete gameObject;
    }
    if (_immediateContext) { _immediateContext->Release(); }
    if (_device) { _device->Release(); }
    if (_dxgiDevice) { _dxgiDevice->Release(); }
    if (_dxgiFactory) { _dxgiFactory->Release(); }
    if (_frameBufferView) { _frameBufferView->Release(); }
    if (_swapChain) { _swapChain->Release(); }
    if (_wireframeState) { _wireframeState->Release(); }
    if (_fillState) { _fillState->Release(); }
    if (_vertexShader) { _vertexShader->Release(); }
    if (_vertexShaderSkybox) { _vertexShaderSkybox->Release(); }
    if (_pixelShader) { _pixelShader->Release(); }
    if (_pixelShaderSkybox) { _pixelShaderSkybox->Release(); }
    if (_inputLayout) { _inputLayout->Release(); }
    if (_constantBuffer) { _constantBuffer->Release(); }
    if (_depthStencilBuffer) { _depthStencilBuffer->Release(); }
    if (_depthStencilView) { _depthStencilView->Release(); }
}

/// <summary>
/// A json loader that specifically loads the lighting data
/// </summary>
void DX11Framework::InitLighting()
{
    //Json Parser
    json jFile;

    std::ifstream fileOpen("JSON/Lighting.json");

    //validates to see if the file has been opened
    if (!fileOpen.is_open() || fileOpen.fail())
    {
        std::cerr << "Error: file could not be opened." << std::endl;
        return;
    }

    jFile = json::parse(fileOpen);
    std::string v = jFile["version"].get<std::string>();
    json& fileData = jFile["Lighting"]; //← gets an array
    int size = fileData.size();

    std::vector<lightingData> lightingdata;

    for (unsigned int i = 0; i < size; i++)
    {
        lightingData ld;
        json& lightsDesc = fileData.at(i);
        ld.lightFile = lightsDesc["Type"]; // ← gets a string
        ld.light.x = lightsDesc["Light"][0];
        ld.light.y = lightsDesc["Light"][1];
        ld.light.z = lightsDesc["Light"][2];
        ld.light.w = lightsDesc["Light"][3];

        ld.material.x = lightsDesc["Material"][0];
        ld.material.y = lightsDesc["Material"][1];
        ld.material.z = lightsDesc["Material"][2];
        ld.material.w = lightsDesc["Material"][3];

        if (ld.lightFile == "diffuse")
        {
            _cbData.diffuseLight = ld.light;
            _cbData.diffuseMaterial = ld.material;
        }
        if (ld.lightFile == "ambient")
        {
            _cbData.ambientLight = ld.light;
            _cbData.ambientMaterial = ld.material;
        }
        if (ld.lightFile == "specular")
        {
            _cbData.specularLight = ld.light;
            _cbData.specularMaterial = ld.material;
        }

        lightingdata.push_back(ld);
    }
    
    fileOpen.close();
}

/// <summary>
/// A json loader that specifically loads the Game Object data
/// </summary>
void DX11Framework::InitGameObjects()
{
    json jFile; //Json Parser

    std::ifstream fileOpen("JSON/GameObjects.json");

    //validates to see if the file has been opened
    if (!fileOpen.is_open() || fileOpen.fail())
    {
        std::cerr << "Error: file could not be opened." << std::endl;
        return;
    }

    jFile = json::parse(fileOpen);

    std::string v = jFile["version"].get<std::string>();
    json& fileData = jFile["GameObjects"]; //← gets an array
    int size = fileData.size();
    gameObjectData g;

    for (unsigned int i = 0; i < size; i++)
    {
        json& gameObjectDesc = fileData.at(i);
        g.objFilePath = gameObjectDesc["FilePath"];
        g.type = gameObjectDesc["Type"];
        g.colorTexture = gameObjectDesc["Color"];
        g.specularTexture = gameObjectDesc["Specular"];
        g.scale.x = gameObjectDesc["Scale"][0];
        g.scale.y = gameObjectDesc["Scale"][1];
        g.scale.z = gameObjectDesc["Scale"][2];
        g.rotation.x = gameObjectDesc["Rotation"][0];
        g.rotation.y = gameObjectDesc["Rotation"][1];
        g.rotation.z = gameObjectDesc["Rotation"][2];
        g.position.x = gameObjectDesc["Position"][0];
        g.position.y = gameObjectDesc["Position"][1];
        g.position.z = gameObjectDesc["Position"][2];

        _gameObjectDataList.push_back(g); //Adds the gameobject to the list
    }

    for (int i = 0; i < _gameObjectDataList.size(); i++)
    {
        //Type
        _gameObject[i].SetType(_gameObjectDataList.at(i).type);

        //Texture
        ID3D11ShaderResourceView* _texture;
        std::wstring colorTexFilePath = static_cast<CString>(_gameObjectDataList.at(i).specularTexture.c_str()).GetString(); //converts it to a wstring, so that it can be converted to a texture
        CreateDDSTextureFromFile(_device, colorTexFilePath.c_str(), nullptr, &_texture);

        //Specular Texture
        std::wstring specTexFilePath = static_cast<CString>(_gameObjectDataList.at(i).colorTexture.c_str()).GetString();
        CreateDDSTextureFromFile(_device, specTexFilePath.c_str(), nullptr, &_texture);

        //Appearance
        Appearance* _appearance = new Appearance(OBJLoader::Load(_gameObjectDataList.at(i).objFilePath, _device, false));
        _appearance->SetTexture(_texture);
        _gameObject[i].SetAppearance(_appearance);

        //Transform
        _gameObject[i].GetTransform()->SetScale(Vector3(_gameObjectDataList.at(i).scale.x, _gameObjectDataList.at(i).scale.y, _gameObjectDataList.at(i).scale.z));
        _gameObject[i].GetTransform()->SetRotation(Vector3(_gameObjectDataList.at(i).rotation.x, _gameObjectDataList.at(i).rotation.y, _gameObjectDataList.at(i).rotation.z));
        _gameObject[i].GetTransform()->SetPosition(Vector3(_gameObjectDataList.at(i).position.x, _gameObjectDataList.at(i).position.y, _gameObjectDataList.at(i).position.z));

        _gameObjects.push_back(&_gameObject[i]); //Adds it a different list with all gameObjects
    }
}

void DX11Framework::Update()
{
    _timer->Tick();

    //Static initializes this value only once    
    static ULONGLONG frameStart = GetTickCount64();

    ULONGLONG frameNow = GetTickCount64();
    float deltaTime = (frameNow - frameStart) / 1000.0f;
    frameStart = frameNow;

    static float accumulator = 0.0f;
    accumulator += deltaTime;

    while (accumulator >= FPS60)
    {
        DebugPrintF("deltaTime is %f\n", accumulator);
        PhysicsUpdates(0.016);
        accumulator -= 0.016; //resets the accumulator counter
    }

    const double alpha = accumulator / 0.016;
    RendererUpdates(alpha);

    //Defines the world position
    XMStoreFloat4x4(&_worldMatrix, XMMatrixIdentity());

    _cbData.cameraPosition = _camera->GetEye();
    _cbData.specPower = 10;
    _cbData.lightDir = XMFLOAT3(0, 0.0f, -1.0f);

    Keybinds();
}

void DX11Framework::RendererUpdates(float deltaTime)
{
    // Update camera
    float angleAroundZ = XMConvertToRadians(_cameraOrbitAngleXZ);

    float x = _cameraOrbitRadius * cos(angleAroundZ);
    float z = _cameraOrbitRadius * sin(angleAroundZ);

    XMFLOAT3 cameraPos = _camera->GetEye();
    cameraPos.x = x;
    cameraPos.z = z;

    _camera->SetEye(cameraPos);
    _camera->Update();

#pragma region CameraMovement
    //W - Fowards
    if (GetAsyncKeyState(0x57) & 0X0001)
    {
        _cameraOrbitRadius = max(_cameraOrbitRadiusMin, _cameraOrbitRadius - (_cameraSpeed * 0.2f));
    }
    //S - Backwards
    if (GetAsyncKeyState(0x53) & 0X0001)
    {
        _cameraOrbitRadius = min(_cameraOrbitRadiusMax, _cameraOrbitRadius + (_cameraSpeed * 0.2f));
    }
    //A - Left
    if (GetAsyncKeyState(0x41) & 0X0001)
    {
        _cameraOrbitAngleXZ += _cameraSpeed;
    }
    //D - Right
    if (GetAsyncKeyState(0x44) & 0X0001)
    {
        _cameraOrbitAngleXZ -= _cameraSpeed;
    }
#pragma endregion
}

void DX11Framework::PhysicsUpdates(float deltaTime)
{
#pragma region GOMovementControls
    //NUMPAD 8 - Fowards
    if (GetAsyncKeyState(0x68) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(0, 0, 4.0f));
        _cubes[1].GetPhysicsModel()->AddForce(Vector3(0, 0, 4.0f));
    }
    //NUMPAD 2 - Backwards
    if (GetAsyncKeyState(0x62) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(0, 0, -4.0f));
        _cubes[1].GetPhysicsModel()->AddForce(Vector3(0, 0, -4.0f));
    }
    //NUMPAD 4 - Left
    if (GetAsyncKeyState(0x64) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(-4.0f, 0, 0));
        _cubes[1].GetPhysicsModel()->AddForce(Vector3(-4.0f, 0, 0));
    }
    //NUMPAD 6 - Right
    if (GetAsyncKeyState(0x66) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(4.0f, 0, 0));
        _cubes[1].GetPhysicsModel()->AddForce(Vector3(4.0f, 0, 0));
    }
    //NUMPAD 9 - Up
    if (GetAsyncKeyState(0x69) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(0, 4.0f, 0));
        _cubes[1].GetPhysicsModel()->AddForce(Vector3(0, 4.0f, 0));
    }
    //NUMPAD 3 - Down
    if (GetAsyncKeyState(0x63) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(0, -4.0f, 0));
        _cubes[1].GetPhysicsModel()->AddForce(Vector3(0, -4.0f, 0));
    }
#pragma endregion

#pragma region GOVelocityControls
    //UP ARROW - Fowards Constant Velocity
    if (GetAsyncKeyState(0x26) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(0, 0, 1), true);
        _cubes[1].GetPhysicsModel()->SetVelocity(Vector3(0, 0, 1), false);
    }
    //DOWN ARROW - Backwards Constant Velocity
    if (GetAsyncKeyState(0x28) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(0, 0, -1), true);
        _cubes[1].GetPhysicsModel()->SetVelocity(Vector3(0, 0, -1), false);
    }
    //LEFT ARROW - Left Constant Velocity
    if (GetAsyncKeyState(0x25) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(-1, 0, 0), true);
        _cubes[1].GetPhysicsModel()->SetVelocity(Vector3(-1, 0, 0), false);
    }
    //RIGHT ARROW - Right Constant Velocity
    if (GetAsyncKeyState(0x27) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(1, 0, 0), true);
        _cubes[1].GetPhysicsModel()->SetVelocity(Vector3(1, 0, 0), false);
    }
    //PAGE UP - Up Constant Velocity
    if (GetAsyncKeyState(0x22) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(0, -1, 0), true);
        _cubes[1].GetPhysicsModel()->SetVelocity(Vector3(0, -1, 0), false);
    }
    //PAGE DOWN - Down Constant Velocity
    if (GetAsyncKeyState(0x21) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(0, 1, 0), true);
        _cubes[1].GetPhysicsModel()->SetVelocity(Vector3(0, 1, 0), false);
    }
#pragma endregion

#pragma region GeneralGOControls
    //END / NUMPAD 1 - Rotation Right
    if (GetAsyncKeyState(0x23) & 0X0001 || GetAsyncKeyState(0x67) & 0X0001)
    {
        //NEED TO ADD
    }
    //HOME / NUMPAD 7 - Rotation Left
    if (GetAsyncKeyState(0x24) & 0X0001 || GetAsyncKeyState(0x61) & 0X0001)
    {
        //NEED TO ADD
    }
    //INSERT / NUMPAD 0 - Stop All Velocity
    if (GetAsyncKeyState(0x2D) & 0X0001 || GetAsyncKeyState(0x60) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(0, 0, 0), true);
        _cubes[1].GetPhysicsModel()->SetVelocity(Vector3(0, 0, 0), false);
    }
#pragma endregion

    //Update objects
    for (GameObject* go : _gameObjects)
    {
        go->Update(deltaTime);
    }
}

void DX11Framework::Keybinds()
{
#pragma region ChangeStates
    //Sets it to fill state on F1 press 
    if (GetAsyncKeyState(VK_F1) & 0x0001)
    {
        _immediateContext->RSSetState(_fillState);
    }

    //Sets it to wireframe state on F2 press 
    if (GetAsyncKeyState(VK_F2) & 0x0001)
    {
        _immediateContext->RSSetState(_wireframeState);
    }
#pragma endregion
}

void DX11Framework::Draw()
{    
    //Present unbinds render target, so rebind and clear at start of each frame
    float backgroundColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };
    _immediateContext->OMSetRenderTargets(1, &_frameBufferView, _depthStencilView);
    _immediateContext->ClearRenderTargetView(_frameBufferView, backgroundColor);
    _immediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    //Vertex and Pixel Shader, Set Shader
    _immediateContext->VSSetShader(_vertexShader, nullptr, 0);
    _immediateContext->PSSetShader(_pixelShader, nullptr, 0);
    _immediateContext->VSSetConstantBuffers(0, 1, &_constantBuffer);
    _immediateContext->PSSetConstantBuffers(0, 1, &_constantBuffer); //this is causing issues??
    _immediateContext->PSSetSamplers(0, 1, &_bilinearSamplerState);

    //Camera
    XMFLOAT4X4 tempView = _camera->GetView();
    XMFLOAT4X4 tempProjection = _camera->GetProjection();

    XMMATRIX view = XMLoadFloat4x4(&tempView);
    XMMATRIX projection = XMLoadFloat4x4(&tempProjection);

    _cbData.View = XMMatrixTranspose(view);
    _cbData.Projection = XMMatrixTranspose(projection);

    //Store this frames data in constant buffer struct
    _cbData.World = XMMatrixTranspose(XMLoadFloat4x4(&_worldMatrix));

    //Write constant buffer data onto GPU
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;

    //Loads Game Objects
    for (auto gameObject : _gameObjects)
    {
        if (gameObject->GetAppearance()->HasTexture())
        {
            _immediateContext->PSSetShaderResources(0, 1, gameObject->GetAppearance()->GetTexture());
            _cbData.hasTexture = 1.0f;
            _cbData.hasSpecularMap = 1.0f;
        }
        else
        {
            _cbData.hasTexture = 0.0f;
            _cbData.hasSpecularMap = 0.0f;
        }
        _cbData.World = XMMatrixTranspose(gameObject->GetTransform()->GetWorldMatrix());
        _immediateContext->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
        _immediateContext->Unmap(_constantBuffer, 0);

        gameObject->Draw(_immediateContext);
    }
    //Skybox
    //Changes the Stencil State to the Skybox one
    _immediateContext->OMSetDepthStencilState(_depthStencilSkybox, 0);

    //Vertex and Pixel Shader, Set Shader
    _immediateContext->VSSetShader(_vertexShaderSkybox, nullptr, 0);
    _immediateContext->PSSetShader(_pixelShaderSkybox, nullptr, 0);

    _cbData.World = XMMatrixTranspose(XMLoadFloat4x4(&_worldMatrix));
    _immediateContext->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
    memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
    _immediateContext->Unmap(_constantBuffer, 0);

    _skybox->Draw(_immediateContext);
    
    //Present back buffer to screen
    _swapChain->Present(0, 0);
}