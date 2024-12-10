#include "DX11Framework.h"
#include "Line.h"
#include "Cube.h"
#include "Pyramid.h"

//#define RETURNFAIL(x) if(FAILED(x)) return x;
GameObject* _gameObject;
//GameObject* donut;
//GameObject star;
//
////Line line;
//Cube skybox;
//Cube cube;
//Pyramid pyramid;

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

    hr = CreateWindowHandle(hInstance, nShowCmd);
    if (FAILED(hr))
    {
        return E_FAIL;
    }

    hr = CreateD3DDevice();
    if (FAILED(hr)) 
    {
        return E_FAIL;
    }

    hr = CreateSwapChainAndFrameBuffer();
    if (FAILED(hr)) 
    {
        return E_FAIL;
    }

    hr = InitShadersAndInputLayout();
    if (FAILED(hr))
    {
        return E_FAIL;
    }

    hr = InitVertexIndexBuffers();
    if (FAILED(hr))
    {
        return E_FAIL;
    }

    hr = InitPipelineVariables();
    if (FAILED(hr))
    {
        return E_FAIL;
    }

    hr = InitRunTimeData();
    if (FAILED(hr))
    {
        return E_FAIL;
    }

    return hr;
}

HRESULT DX11Framework::CreateWindowHandle(HINSTANCE hInstance, int nCmdShow)
{
    const wchar_t* windowName  = L"DX11Framework";

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
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //FLIP* modes don't support sRGB backbuffer
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

    //skybox vertex shader
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

    //other
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

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
    };

    hr = _device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &_inputLayout);
    if (FAILED(hr))
    {
        return hr;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3DBlob* psBlob;

    //skybox pixel shader
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

    //other
    hr = D3DCompileFromFile(L"SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0", dwShaderFlags, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    hr = _device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &_pixelShader);

    vsBlob->Release();
    psBlob->Release();

    return hr;
}

HRESULT DX11Framework::InitVertexIndexBuffers()
{
    HRESULT hr = S_OK;

    //line.VertexData(_device);

    //skybox.VertexData(_device);
    //skybox.IndexData(_device, true);

    //cube.VertexData(_device);
    //cube.IndexData(_device, false);

    //pyramid.VertexData(_device);
    //pyramid.IndexData(_device);

    return S_OK;
}

HRESULT DX11Framework::InitPipelineVariables()
{
    HRESULT hr = S_OK;

    //Input Assembler
    _immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _immediateContext->IASetInputLayout(_inputLayout);

    //Rasterizer
    D3D11_RASTERIZER_DESC fillDesc = {};
    fillDesc.FillMode = D3D11_FILL_SOLID;
    fillDesc.CullMode = D3D11_CULL_BACK;

    hr = _device->CreateRasterizerState(&fillDesc, &_fillState);
    if (FAILED(hr))
    {
        return hr;
    }

    //Wireframe
    D3D11_RASTERIZER_DESC wireframeDesc = {};
    wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
    wireframeDesc.CullMode = D3D11_CULL_NONE;

    hr = _device->CreateRasterizerState(&wireframeDesc, &_wireframeState);
    if (FAILED(hr))
    {
        return hr;
    }

    //Viewport Values
    D3D11_VIEWPORT _viewport = Camera().GetViewPort();
    _immediateContext->RSSetViewports(1, &_viewport);

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

    _immediateContext->PSSetSamplers(0, 1, &_bilinearSamplerState);

    //Skybox
    D3D11_DEPTH_STENCIL_DESC dsDescSkybox = { };
    dsDescSkybox.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    dsDescSkybox.DepthEnable = true;
    dsDescSkybox.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

    hr = _device->CreateDepthStencilState(&dsDescSkybox, &_depthStencilSkybox);
    if (FAILED(hr))
    {
        return hr;
    }

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

    _immediateContext->VSSetConstantBuffers(0, 1, &_constantBuffer);
    _immediateContext->PSSetConstantBuffers(0, 1, &_constantBuffer);

    return S_OK;
}

HRESULT DX11Framework::InitRunTimeData()
{
    HRESULT hr = S_OK;

    //Camera Settings - This is where they are initalised
    camNumber = 0;

    _view = camera[0].GetView();
    _projection = camera[0].GetProjection();

    camera[0].SetEye(XMFLOAT3(0, 0, -6.1));
    camera[0].SetAt(XMFLOAT3(0, 0, 0));
    camera[0].SetUp(XMFLOAT3(0, 1, 0));

    camera[1].SetEye(XMFLOAT3(0, 0, -20.1));
    camera[1].SetAt(XMFLOAT3(0, 0, 0));
    camera[1].SetUp(XMFLOAT3(0, 1, 0));

    camera[2].SetEye(XMFLOAT3(0, 0, -6.1));
    camera[2].SetAt(XMFLOAT3(0, 0, 0));
    camera[2].SetUp(XMFLOAT3(0, 1, 0));

    LoadGameObjects();

    //Textures
    //hr = CreateDDSTextureFromFile(_device, L"Textures\\skybox.dds", nullptr, &_skyboxTexture);
    //hr = CreateDDSTextureFromFile(_device, L"Textures\\Crate_SPEC.dds", nullptr, &_crateTexture);
    //hr = CreateDDSTextureFromFile(_device, L"Textures\\Crate_COLOR.dds", nullptr, &_crateTexture);

    //_hasSpecularMap = 1;
    //_hasTexture = 1;

    //Meshes
    /*donutOBJData = OBJLoader::Load("Models\\Test Models\\Made in Blender\\donut.obj", _device, false);
    hr = CreateDDSTextureFromFile(_device, L"Textures\\asphalt_SPEC.dds", nullptr, &_asphaltTexture);
    hr = CreateDDSTextureFromFile(_device, L"Textures\\asphalt_COLOR.dds", nullptr, &_asphaltTexture);
    donut->SetMeshData(donutOBJData);
    donut->SetShaderResource(_asphaltTexture);

    starOBJData = OBJLoader::Load("Models\\Test Models\\Made in 3ds Max\\star.obj", _device);*/

    if (FAILED(hr))
    {
        return hr;
    }
}

void DX11Framework::Keybinds()
{
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

    //Switch Cameras
    //Cam 0 - Basic Static Cam
    if (GetAsyncKeyState(0x31) & 0x0001)
    {
        camNumber = 0;
    }

    //Cam 1 - Basic Static Cam 2
    if (GetAsyncKeyState(0x32) & 0x0001)
    {
        camNumber = 1;
    }

    //Cam 2 - FreeCam
    if (GetAsyncKeyState(0x33) & 0x0001)
    {
        camNumber = 2;
    }

    if (camNumber == 2)
    {
        //FreeCam Movement
        //W - Fowards
        if (GetAsyncKeyState(0x57) & 0X0001)
        {
            _eyeMovement = camera[2].GetEye();
            _operator = XMFLOAT3(0, 0, 1.0f);
            _vector = XMLoadFloat3(&_eyeMovement);
            _operation = XMLoadFloat3(&_operator);
            _eyeResult = XMVectorAdd(_vector, _operation);
            XMStoreFloat3(&_eyeMovement, _eyeResult);

            _lookAt = camera[2].GetAt();
            _operator = XMFLOAT3(0, 0, 1.0f);
            _vector = XMLoadFloat3(&_lookAt);
            _operation = XMLoadFloat3(&_operator);
            _lookAtResult = XMVectorAdd(_vector, _operation);
            XMStoreFloat3(&_lookAt, _lookAtResult);

            _View = camera[2].GetView();
            _Projection = camera[2].GetProjection();

            camera[2].SetEye(XMFLOAT3(_eyeMovement.x, _eyeMovement.y, _eyeMovement.z));
            camera[2].SetAt(XMFLOAT3(_lookAt.x, _lookAt.y, _lookAt.z));
            return;
        }

        //S - Backwards
        if (GetAsyncKeyState(0x53) & 0X0001)
        {
            _eyeMovement = camera[2].GetEye();
            _operator = XMFLOAT3(0, 0, -1.0f);
            _vector = XMLoadFloat3(&_eyeMovement);
            _operation = XMLoadFloat3(&_operator);
            _eyeResult = XMVectorAdd(_vector, _operation);
            XMStoreFloat3(&_eyeMovement, _eyeResult);

            _lookAt = camera[2].GetAt();
            _operator = XMFLOAT3(0, 0, -1.0f);
            _vector = XMLoadFloat3(&_lookAt);
            _operation = XMLoadFloat3(&_operator);
            _lookAtResult = XMVectorAdd(_vector, _operation);
            XMStoreFloat3(&_lookAt, _lookAtResult);

            _View = camera[2].GetView();
            _Projection = camera[2].GetProjection();

            camera[2].SetEye(XMFLOAT3(_eyeMovement.x, _eyeMovement.y, _eyeMovement.z));
            camera[2].SetAt(XMFLOAT3(_lookAt.x, _lookAt.y, _lookAt.z));
            return;
        }
    }

    //Resets the FreeCam settings if FreeCam isnt active
    else if (camNumber != 2)
    {
        camera[2].SetEye(XMFLOAT3(0, 0, -6.1));
        camera[2].SetAt(XMFLOAT3(0, 0, 0));
        camera[2].SetUp(XMFLOAT3(0, 1, 0));
    }
}

DX11Framework::~DX11Framework()
{
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
    if (_inputLayout) { _inputLayout->Release(); }
    if (_constantBuffer) { _constantBuffer->Release(); }
    if (_depthStencilBuffer) { _depthStencilBuffer->Release(); }
    if (_depthStencilView) { _depthStencilView->Release(); }
}

/// <summary>
/// A json loader that specifically loads the lighting data
/// </summary>
void DX11Framework::LoadLightingData()
{
    //Json Parser
    json jFile;

    std::ifstream fileOpen("JSON/lighting.json");

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

void DX11Framework::LoadGameObjects()
{
    json jFile; //Json Parser

    std::ifstream fileOpen("JSON/gameObjects.json");

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

    for (unsigned int i = 0; i < size; i++)
    {
        gameObjectData g;
        json& gameObjectDesc = fileData.at(i);
        g.objFilePath = gameObjectDesc["FilePath"];
        g.specularTexture = gameObjectDesc["Specular"];
        g.colorTexture = gameObjectDesc["Color"];
        g.startScale.x = gameObjectDesc["StartScale"][0];
        g.startScale.y = gameObjectDesc["StartScale"][1];
        g.startScale.z = gameObjectDesc["StartScale"][2];
        g.startRot.x = gameObjectDesc["StartRot"][0];
        g.startRot.y = gameObjectDesc["StartRot"][1];
        g.startRot.z = gameObjectDesc["StartRot"][2];
        g.startPos.x = gameObjectDesc["StartPos"][0];
        g.startPos.y = gameObjectDesc["StartPos"][1];
        g.startPos.z = gameObjectDesc["StartPos"][2];

        objects.push_back(g); //Adds the gameobject to the list
    }
}

void DX11Framework::Update()
{
    //Sets the view and projection to the currently active camera
    _view = camera[camNumber].GetView();
    _projection = camera[camNumber].GetProjection();

    _cbData.View = XMMatrixTranspose(XMLoadFloat4x4(&_view));
    _cbData.Projection = XMMatrixTranspose(XMLoadFloat4x4(&_projection));

    //Cameras
    camera[camNumber].Update();

    //Static initializes this value only once    
    static ULONGLONG frameStart = GetTickCount64();

    ULONGLONG frameNow = GetTickCount64();
    float deltaTime = (frameNow - frameStart) / 1000.0f;
    frameStart = frameNow;

    static float simpleCount = 0.0f;
    simpleCount += deltaTime;

    static float fastCount = 0.0f;
    fastCount += (deltaTime * 2);

    //Defines the objects world position
    XMStoreFloat4x4(&_worldMatrix, XMMatrixIdentity() * XMMatrixRotationX(simpleCount));
    /*XMStoreFloat4x4(&_skyboxMatrix, XMMatrixIdentity() * XMMatrixScaling(75, 75, 75));
    XMStoreFloat4x4(&_cubeWorldMatrix, XMMatrixIdentity() * XMMatrixScaling(0.5, 0.5, 0.5) * XMMatrixTranslation(0, 0, 3) * XMMatrixRotationY(simpleCount));
    XMStoreFloat4x4(&_lineWorldMatrix, XMMatrixIdentity() * XMMatrixTranslation(0, 0, 0));
    XMStoreFloat4x4(&_pyramidWorldMatrix, XMMatrixIdentity() * XMMatrixScaling(0.25, 0.25, 0.25) * XMMatrixTranslation(0, 0, 4.25) * XMMatrixRotationY(simpleCount));
    XMStoreFloat4x4(&_starOBJWorldMatrix, XMMatrixIdentity() * XMMatrixScaling(1, 1, 1) * XMMatrixTranslation(0, 0, 4) * (XMMatrixRotationX(simpleCount) * XMMatrixRotationY(simpleCount)));
    XMStoreFloat4x4(&_donutOBJWorldMatrix, XMMatrixIdentity() * XMMatrixScaling(2.2, 2.2, 2.2) * (XMMatrixRotationY(fastCount) * XMMatrixRotationX(fastCount)));*/

    //star.SetWorldMatrix(_starOBJWorldMatrix); //pass the world matrix into the GameObject Class
    //donut->SetWorldMatrix(_donutOBJWorldMatrix);

    //Lighting
    LoadLightingData();
    _cbData.cameraPosition = camera[0].GetEye();
    _cbData.specPower = 10;
    _cbData.lightDir = XMFLOAT3(0, 0.0f, -1.0f);
    _cbData.hasTexture = _hasTexture;
    _cbData.hasSpecularMap = _hasSpecularMap;
    _cbData.count = simpleCount;

    Keybinds();

    // Update objects
    //for (int i = 0; i < objects.size(); i++)
    {
        //gameObject->Update(deltaTime);
    }
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

    //Store this frames data in constant buffer struct
    _cbData.World = XMMatrixTranspose(XMLoadFloat4x4(&_worldMatrix));
    
    D3D11_MAPPED_SUBRESOURCE mappedSubresource; //Write constant buffer data onto GPU

    for (int i = 0; i < objects.size(); i++)
    {
        GameObject* _gameObject = new GameObject;
        _gameObject->SetMeshData(OBJLoader::Load(objects.at(i).objFilePath, _device, false)); //pass the meshData into the GameObject Class

        //CreateDDSTextureFromFile(_device, objects.at(i).specularTexture, nullptr, &_texture);
        //CreateDDSTextureFromFile(_device, objects.at(i).colorTexture, nullptr, &_texture);
        //gameObject->SetShaderResource(_texture);

        _gameObject->SetScale(XMFLOAT3(objects.at(i).startScale.x, objects.at(i).startScale.y, objects.at(i).startScale.z));
        _gameObject->SetRotation(XMFLOAT3(objects.at(i).startRot.x, objects.at(i).startRot.y, objects.at(i).startRot.z));
        _gameObject->SetPosition(XMFLOAT3(objects.at(i).startPos.x, objects.at(i).startPos.y, objects.at(i).startPos.y));

        _gameObject->SetGameObject(_gameObject);

        _immediateContext->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

        _cbData.World = XMMatrixTranspose(XMLoadFloat4x4(&_worldMatrix));

        memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
        _immediateContext->Unmap(_constantBuffer, 0);

        _gameObject->Draw(_immediateContext);
    }
    
    //Present Backbuffer to screen
    _swapChain->Present(0, 0);
}