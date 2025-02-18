#include "DX11Framework.h"

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

    ID3DBlob* vsBlobSkybox;
    ID3DBlob* psBlobSkybox;

    // Skybox
    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC skyboxInputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    // Compile the vertex shader
    hr = D3DCompileFromFile(L"SkyboxShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_main", "vs_5_0", dwShaderFlags, 0, &vsBlobSkybox, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    // Create the vertex shader
    hr = _device->CreateVertexShader(vsBlobSkybox->GetBufferPointer(), vsBlobSkybox->GetBufferSize(), nullptr, &_skyboxVertexShader);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    // Compile the pixel shader
    hr = D3DCompileFromFile(L"SkyboxShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0", dwShaderFlags, 0, &psBlobSkybox, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    // Create the pixel shader
    hr = _device->CreatePixelShader(psBlobSkybox->GetBufferPointer(), psBlobSkybox->GetBufferSize(), nullptr, &_skyboxPixelShader);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    // Create the input layout
    hr = _device->CreateInputLayout(skyboxInputElementDesc, ARRAYSIZE(skyboxInputElementDesc), vsBlobSkybox->GetBufferPointer(), vsBlobSkybox->GetBufferSize(), &_skyboxInputLayout);
    if (FAILED(hr)) { return hr; }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ID3DBlob* vsBlob;
    ID3DBlob* psBlob;

    // Standard
    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    // Compile the vertex shader
    hr = D3DCompileFromFile(L"SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_main", "vs_5_0", dwShaderFlags, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    // Create the vertex shader
    hr = _device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &_vertexShader);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    // Compile the pixel shader
    hr = D3DCompileFromFile(L"SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0", dwShaderFlags, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    // Create the pixel shader
    hr = _device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &_pixelShader);
    if (FAILED(hr))
    {
        MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    //Create the input layout
    hr = _device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &_inputLayout);
    if (FAILED(hr)) { return hr; }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    vsBlobSkybox->Release();
    psBlobSkybox->Release();
    vsBlob->Release();
    psBlob->Release();
    errorBlob->Release();

    return hr;
}

HRESULT DX11Framework::InitPipelineVariables()
{
    HRESULT hr = S_OK;

    // Input Assembler
    _immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Standard Depth Stencil State
    D3D11_DEPTH_STENCIL_DESC dssDesc;
    ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dssDesc.DepthEnable = true;
    dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    hr = _device->CreateDepthStencilState(&dssDesc, &_DSLessEqual);
    if (FAILED(hr)) { return hr; }

    // Skybox Depth Stencil State
    D3D11_DEPTH_STENCIL_DESC dsDescSkybox = { };
    dsDescSkybox.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    dsDescSkybox.DepthEnable = true;
    dsDescSkybox.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

    hr = _device->CreateDepthStencilState(&dsDescSkybox, &_skyboxDepthStencil);
    if (FAILED(hr)) { return hr; }

    // Rasterizer
    // Wireframe State
    D3D11_RASTERIZER_DESC cmdesc;
    ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
    cmdesc.FillMode = D3D11_FILL_WIREFRAME;
    cmdesc.CullMode = D3D11_CULL_NONE;
    hr = _device->CreateRasterizerState(&cmdesc, &_wireframeState);
    if (FAILED(hr)) { return hr; }

    // Fill State
    ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
    cmdesc.FillMode = D3D11_FILL_SOLID;
    cmdesc.CullMode = D3D11_CULL_BACK;
    cmdesc.FrontCounterClockwise = true;
    hr = _device->CreateRasterizerState(&cmdesc, &_fillState);
    if (FAILED(hr)) { return hr; }

    cmdesc.FrontCounterClockwise = false;
    hr = _device->CreateRasterizerState(&cmdesc, &_fillState);

    _immediateContext->RSSetState(_fillState);

    // Bilinear Sampler
    D3D11_SAMPLER_DESC bilinearSampledesc = {};
    bilinearSampledesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    bilinearSampledesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    bilinearSampledesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    bilinearSampledesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    bilinearSampledesc.MaxLOD = D3D11_FLOAT32_MAX;
    bilinearSampledesc.MinLOD = 0;

    hr = _device->CreateSamplerState(&bilinearSampledesc, &_bilinearSamplerState);
    if (FAILED(hr)) { return hr; }

    return S_OK;
}

HRESULT DX11Framework::InitRunTimeData()
{
    HRESULT hr = S_OK;

    // Constant Buffer
    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    _viewport = { 0.0f, 0.0f, (float)_WindowWidth, (float)_WindowHeight, 0.0f, 1.0f };
    _immediateContext->RSSetViewports(1, &_viewport);

    hr = _device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffer);
    if (FAILED(hr)) { return hr; }

    // Setup Camera
    XMFLOAT3 eye = XMFLOAT3(0.0f, 2.0f, -1.0f);
    XMFLOAT3 at = XMFLOAT3(0.0f, 2.0f, 0.0f);
    XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

    _camera = new Camera(eye, at, up, (float)_WindowWidth, (float)_WindowHeight, 0.01f, 200.0f);

    // Lighting
    InitLighting();

    // Initiate Scene
    ID3D11ShaderResourceView* _texture;
    Mesh* _mesh = new Mesh();
    Appearance* _appearance;
    PhysicsModel* _physicsModel;
    Collider* _collider;

    // Skybox
    // Mesh Initialisation
    _mesh = new Mesh();
    _mesh->SetMeshData(_mesh->CreateInvertedCube());
    _appearance = new Appearance(_mesh);

    // Texture Initialisation
    CreateDDSTextureFromFile(_device, L"Textures\\Free Assets Online\\spyro3Skybox.dds", nullptr, &_texture);
    if (FAILED(hr)) { return hr; }
    _appearance->SetTexture(_texture);

    // Set Type and Appearance
    _skybox->SetType("Skybox");
    _skybox->SetAppearance(_appearance);

    // Floor
    // Mesh Initialisation
    _mesh = new Mesh();
    _mesh->SetMeshData(_mesh->CreatePlane());
    _appearance = new Appearance(_mesh);

    // Texture Initialisation
    CreateDDSTextureFromFile(_device, L"Textures\\Test Textures\\floor.dds", nullptr, &_texture);
    if (FAILED(hr)) { return hr; }
    _appearance->SetTexture(_texture);

    // Set Type and Appearance
    _floor->SetType("Floor");
    _floor->SetAppearance(_appearance);

    // Transform Initialisation
    _floor->GetTransform()->SetPosition(Vector3(0.0f, 0.1f, 0.0f));
    _floor->GetTransform()->SetScale(Vector3(15.0f, 15.0f, 15.0f));
    _floor->GetTransform()->SetRotation(Vector3(90.0f, 0.0f, 0.0f));

    // Plane Collider Initialisation
    //_collider = new BoxCollider(_floor->GetTransform());
    //_floor->GetPhysicsModel()->SetCollider(_collider);

    _gameObjects.push_back(_floor);

    // Cubes
    for (auto i = 0; i < 4; i++)
    {
        // Mesh Initialisation
        _mesh = new Mesh();
        _mesh->SetMeshData(_mesh->CreateCube());
        _appearance = new Appearance(_mesh);

        // Texture Initialisation
        CreateDDSTextureFromFile(_device, L"Textures\\Test Textures\\stone.dds", nullptr, &_texture);
        if (FAILED(hr)) { return hr; }
        _appearance->SetTexture(_texture);

        // Set Type and Appearance
        _cubes[i].SetType("Cube " + i);
        _cubes[i].SetAppearance(_appearance);

        // Transform Initialisation
        _cubes[i].GetTransform()->SetPosition(Vector3(-2.0f + (i * 2.5f), 2.0f, 10.0f));
        _cubes[i].GetTransform()->SetScale(Vector3(1.0f, 1.0f, 1.0f));

        // Physics Model and Mass Initialisation
        _physicsModel = new RigidBodyModel(_cubes[i].GetTransform());
        _cubes[i].SetPhysicsModel(_physicsModel);
        _cubes[i].GetPhysicsModel()->SetMass(1.0f);

        // Sphere Collider Initialisation
        _collider = new SphereCollider(_cubes[i].GetTransform(), 1.0f);
        _cubes[i].GetPhysicsModel()->SetCollider(_collider);

        // Box Collider Initialisation
        _collider = new BoxCollider(_cubes[i].GetTransform());
        _cubes[i].GetPhysicsModel()->SetCollider(_collider);
         
        _gameObjects.push_back(&_cubes[i]);
    }
    // GameObjects
    InitGameObjects();
}

DX11Framework::~DX11Framework()
{
    if (_camera) { delete _camera; }
    if (_skybox) { delete _skybox; }
    for (auto& go : _gameObjects)
    {
       delete go;
    }
    if (_immediateContext) { _immediateContext->Release(); }

    if (_device) { _device->Release(); }
    if (_dxgiDevice) { _dxgiDevice->Release(); }
    if (_dxgiFactory) { _dxgiFactory->Release(); }
    if (_frameBufferView) { _frameBufferView->Release(); }
    if (_swapChain) { _swapChain->Release(); }
    if (_wireframeState) { _wireframeState->Release(); }
    if (_fillState) { _fillState->Release(); }
    if (_constantBuffer) { _constantBuffer->Release(); }
    if (_depthStencilBuffer) { _depthStencilBuffer->Release(); }
    if (_depthStencilView) { _depthStencilView->Release(); }

    if (_vertexShader) { _vertexShader->Release(); }
    if (_pixelShader) { _pixelShader->Release(); }
    if (_inputLayout) { _inputLayout->Release(); }

    if (_skyboxVertexShader) { _skyboxVertexShader->Release(); }
    if (_skyboxPixelShader) { _skyboxPixelShader->Release(); }
    if (_skyboxInputLayout) { _skyboxInputLayout->Release(); }
    if (_skyboxDepthStencil) { _skyboxDepthStencil->Release(); }
}

void DX11Framework::InitLighting()
{
    // Json Parser
    json jFile;

    std::ifstream fileOpen("JSON/Lighting.json");

    // Validates to see if the file has been opened
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
        ld.lightFile = lightsDesc["Type"]; //← gets a string
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

void DX11Framework::InitGameObjects()
{
    json jFile;
    std::ifstream fileOpen("JSON/GameObjects.json");

    if (!fileOpen.is_open() || fileOpen.fail())
    {
        std::cerr << "Error: file could not be opened." << std::endl;
        return;
    }

    jFile = json::parse(fileOpen);

    std::string v = jFile["version"].get<std::string>();
    json& fileData = jFile["GameObjects"];
    int size = fileData.size();
    gameObjectData g;

    for (unsigned int i = 0; i < size; i++)
    {
        json& gameObjectDesc = fileData.at(i);
        g.objFilePath = gameObjectDesc["File Path"];
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

        _gameObjectDataList.push_back(g); // Adds the gameobject to the list
    }

    for (int i = 0; i < _gameObjectDataList.size(); i++)
    {
        // Type Initialisation
        _gameObject[i].SetType(_gameObjectDataList.at(i).type);

        // Texture Initialisation
        ID3D11ShaderResourceView* _texture;
        std::wstring colorTexFilePath = static_cast<CString>(_gameObjectDataList.at(i).specularTexture.c_str()).GetString(); // Converts it to a wstring, so that it can be converted to a texture
        CreateDDSTextureFromFile(_device, colorTexFilePath.c_str(), nullptr, &_texture);

        // Specular Texture Initialisation
        std::wstring specTexFilePath = static_cast<CString>(_gameObjectDataList.at(i).colorTexture.c_str()).GetString();
        CreateDDSTextureFromFile(_device, specTexFilePath.c_str(), nullptr, &_texture);

        // Mesh Initialisation
        Mesh* _mesh = new Mesh(); 
        _mesh->SetMeshData(new MeshData(OBJLoader::Load(_gameObjectDataList.at(i).objFilePath, _device, false)));

        // Appearance Initialisation
        Appearance* _appearance = new Appearance(_mesh);
        _appearance->SetTexture(_texture);
        _gameObject[i].SetAppearance(_appearance);
        
        // Transform Initialisation
        _gameObject[i].GetTransform()->SetScale(Vector3(_gameObjectDataList.at(i).scale.x, _gameObjectDataList.at(i).scale.y, _gameObjectDataList.at(i).scale.z));
        _gameObject[i].GetTransform()->SetRotation(Vector3(_gameObjectDataList.at(i).rotation.x, _gameObjectDataList.at(i).rotation.y, _gameObjectDataList.at(i).rotation.z));
        _gameObject[i].GetTransform()->SetPosition(Vector3(_gameObjectDataList.at(i).position.x, _gameObjectDataList.at(i).position.y, _gameObjectDataList.at(i).position.z));

        // Adds it a different list with all gameObjects
        _gameObjects.push_back(&_gameObject[i]); 
    }
}

void DX11Framework::Update()
{
    _timer->Tick();

    // Static initializes this value only once    
    static ULONGLONG frameStart = GetTickCount64();

    ULONGLONG frameNow = GetTickCount64();
    float deltaTime = (frameNow - frameStart) / 1000.0f;
    frameStart = frameNow;

    static float accumulator = 0.0f;
    accumulator += deltaTime;

    while (accumulator >= FPS60)
    {
        accumulator -= FPS60; // resets the accumulator counter

        PhysicsUpdates(FPS60);
        //DebugPrintF("DeltaTime is %f\n The number is %i\n", accumulator, 2);
    }

    const double alpha = accumulator / 0.016;
    RendererUpdates(alpha);

    // Defines the world position
    XMStoreFloat4x4(&_worldMatrix, XMMatrixIdentity());

    _cbData.cameraPosition = _camera->GetEye();
    _cbData.specPower = 10;
    _cbData.lightDir = XMFLOAT3(0.0f, 0.5f, -1.0f);

    CollisionManager();
    Keybinds();
}

void DX11Framework::CollisionManager()
{
    for (auto& go : _gameObjects)
    {
        ResolveCollisions(go, _floor);
    }

    // Cube Collisions
    ResolveCollisions(&_cubes[0], &_cubes[1]);
    ResolveCollisions(&_cubes[0], &_cubes[2]);
    ResolveCollisions(&_cubes[0], &_cubes[3]);
    ResolveCollisions(&_cubes[1], &_cubes[2]);
    ResolveCollisions(&_cubes[1], &_cubes[3]);
    ResolveCollisions(&_cubes[2], &_cubes[3]);
}

void DX11Framework::ResolveCollisions(GameObject* obj1, GameObject* obj2)
{
    CollisionManifold manifold;

    Transform* objATransform = obj1->GetTransform();
    Transform* objBTransform = obj2->GetTransform();

    PhysicsModel* objA = obj1->GetPhysicsModel();
    PhysicsModel* objB = obj2->GetPhysicsModel();

    if (objA->IsCollideable() && objB->IsCollideable() && objA->GetCollider()->CollidesWith(*objB->GetCollider(), manifold))
    {
        // Normalise Calculation
        Vector3 collisionNormal = objATransform->GetPosition() - objBTransform->GetPosition();
        collisionNormal.Normalize();

        //Vector3 collisionNormal = manifold.collisionNormal; //This does not work for some reason

        // Velocity Calculation
        Vector3 relativeVelocity = objA->GetVelocity() - objB->GetVelocity();

        // Inverse Mass
        float invMassA = objA->GetInverseMass();
        float invMassB = objB->GetInverseMass();
        float invMassSum = invMassA + invMassB;

        if (collisionNormal * relativeVelocity < 0.0f)
        {
            // This currently isnt used (-1 + restitution)
            float restitution = 0.2f;

            // Total Velocity of Collision = Coefficient of Restitution * Dot Product
            float vj = collisionNormal * relativeVelocity;

            // Conservation of Momentum (Impulse) = Divide the velocity of the impulse by the sum of the inverse masses of the objects
            float j = vj / invMassSum;

            //// This only applies if both objects has a sphere collider (DOESNT WORK YET)
            //if (objA->GetCollider()->GetRadius() > 0.0f && objB->GetCollider()->GetRadius() > 0.0f)
            //{
            //    float radiiSum = objA->GetCollider()->GetRadius() + objB->GetCollider()->GetRadius();
            //    float depth = (objATransform->GetPosition() - objBTransform->GetPosition()) - radiiSum;

            //    // Linear Velocity
            //    objA->ApplyImpulse(-(invMassA * j * collisionNormal * depth));
            //    objB->ApplyImpulse(invMassB * j * collisionNormal * depth); //reversed

            //    DebugPrintF("Collided\n");
            //}
            //else

            // Linear Velocity
            objA->ApplyImpulse(-(invMassA * j * collisionNormal));
            objB->ApplyImpulse(invMassB * j * collisionNormal); //reversed
        }
    }
    // Resets the manifold for the next collision
    manifold = CollisionManifold();
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
    // I - Fowards
    if (GetAsyncKeyState(0x49) & 0X0001)
    {
        _cameraOrbitRadius = max(_cameraOrbitRadiusMin, _cameraOrbitRadius - (_cameraSpeed * 0.2f));
    }
    // K - Backwards
    if (GetAsyncKeyState(0x4B) & 0X0001)
    {
        _cameraOrbitRadius = min(_cameraOrbitRadiusMax, _cameraOrbitRadius + (_cameraSpeed * 0.2f));
    }
    // J - Left
    if (GetAsyncKeyState(0x4A) & 0X0001)
    {
        _cameraOrbitAngleXZ += _cameraSpeed;
    }
    // L - Right
    if (GetAsyncKeyState(0x4C) & 0X0001)
    {
        _cameraOrbitAngleXZ -= _cameraSpeed;
    }
#pragma endregion
}

void DX11Framework::PhysicsUpdates(float deltaTime)
{
#pragma region GOMovementControls
    // W - Fowards
    if (GetAsyncKeyState(0x57) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(0, 0, 4.0f));
        //_cubes[1].GetPhysicsModel()->AddForce(Vector3(0, 0, 4.0f));
    }
    // S - Backwards
    if (GetAsyncKeyState(0x53) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(0, 0, -4.0f));
        //_cubes[1].GetPhysicsModel()->AddForce(Vector3(0, 0, -4.0f));
    }
    // A - Left
    if (GetAsyncKeyState(0x41) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(-4.0f, 0, 0));
        //_cubes[1].GetPhysicsModel()->AddForce(Vector3(-4.0f, 0, 0));
    }
    // D - Right
    if (GetAsyncKeyState(0x44) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(4.0f, 0, 0));
        //_cubes[1].GetPhysicsModel()->AddForce(Vector3(4.0f, 0, 0));
    }
    // Z - Up
    if (GetAsyncKeyState(0x5A) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(0, 4.0f, 0));
        //_cubes[1].GetPhysicsModel()->AddForce(Vector3(0, 4.0f, 0));
    }
    // X - Down
    if (GetAsyncKeyState(0x58) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddForce(Vector3(0, -4.0f, 0));
        //_cubes[1].GetPhysicsModel()->AddForce(Vector3(0, -4.0f, 0));
    }
#pragma endregion

#pragma region GOVelocityControls

    // Note - Currently, Collision DOESNT work if the velocity is set as constant

    // UP ARROW - Fowards Constant Velocity
    if (GetAsyncKeyState(0x26) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(0, 0, 1), true);
    }
    // DOWN ARROW - Backwards Constant Velocity
    if (GetAsyncKeyState(0x28) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(0, 0, -1), true);
    }
    // LEFT ARROW - Left Constant Velocity
    if (GetAsyncKeyState(0x25) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(-1, 0, 0), true);
    }
    // RIGHT ARROW - Right Constant Velocity
    if (GetAsyncKeyState(0x27) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(1, 0, 0), true);
    }
    // PAGE UP - Up Constant Velocity
    if (GetAsyncKeyState(0x22) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(0, -1, 0), true);
    }
    // PAGE DOWN - Down Constant Velocity
    if (GetAsyncKeyState(0x21) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(0, 1, 0), true);
    }
#pragma endregion

#pragma region GeneralGOControls
    // INSERT / NUMPAD 0 - Stop All Velocity
    if (GetAsyncKeyState(0x2D) & 0X0001 || GetAsyncKeyState(0x60) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->SetVelocity(Vector3(0, 0, 0), true);
    }
    // Q - Apply Relative Force
    if (GetAsyncKeyState(0x51) & 0X0001 || GetAsyncKeyState(0x60) & 0X0001)
    {
        _cubes[0].GetPhysicsModel()->AddRelativeForce(Vector3(0, 0, -1), Vector3(1, 0, -1));
    }
#pragma endregion

    // Update objects
    for (GameObject* go : _gameObjects)
    {
        go->Update(deltaTime);
    }
}

void DX11Framework::Keybinds()
{
#pragma region ChangeStates
    // Sets it to fill state on F1 press 
    if (GetAsyncKeyState(VK_F1) & 0x0001)
    {
        _immediateContext->RSSetState(_fillState);
    }

    // Sets it to wireframe state on F2 press 
    if (GetAsyncKeyState(VK_F2) & 0x0001)
    {
        _immediateContext->RSSetState(_wireframeState);
    }
#pragma endregion
}

void DX11Framework::Draw()
{    
    // Sets the Standard Input Assembler and Stencil State
    _immediateContext->IASetInputLayout(_inputLayout);
    _immediateContext->OMSetDepthStencilState(_DSLessEqual, 0);

    // Sets the Standard Vertex and Pixel Shader, and sets Shader
    _immediateContext->VSSetShader(_vertexShader, nullptr, 0);
    _immediateContext->PSSetShader(_pixelShader, nullptr, 0);

    // Present unbinds render target, so rebind and clear at start of each frame
    float backgroundColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };
    _immediateContext->OMSetRenderTargets(1, &_frameBufferView, _depthStencilView);
    _immediateContext->ClearRenderTargetView(_frameBufferView, backgroundColor);
    _immediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    // Camera
    XMFLOAT4X4 tempView = _camera->GetView();
    XMFLOAT4X4 tempProjection = _camera->GetProjection();

    XMMATRIX view = XMLoadFloat4x4(&tempView);
    XMMATRIX projection = XMLoadFloat4x4(&tempProjection);

    _cbData.View = XMMatrixTranspose(view);
    _cbData.Projection = XMMatrixTranspose(projection);

    _immediateContext->VSSetConstantBuffers(0, 1, &_constantBuffer);
    _immediateContext->PSSetConstantBuffers(0, 1, &_constantBuffer);
    _immediateContext->PSSetSamplers(0, 1, &_bilinearSamplerState);

    // Store this frames data in constant buffer struct
    _cbData.World = XMMatrixTranspose(XMLoadFloat4x4(&_worldMatrix));

    // Loads Game Objects
    for (auto gameObject : _gameObjects)
    {
        gameObject->Draw();
    }
    // Skybox
    // Sets the Skybox Input Assembler and Stencil State
    _immediateContext->IASetInputLayout(_skyboxInputLayout);
    _immediateContext->OMSetDepthStencilState(_skyboxDepthStencil, 0);
    
    // Vertex and Pixel Shader, Set Shader
    _immediateContext->VSSetShader(_skyboxVertexShader, nullptr, 0);
    _immediateContext->PSSetShader(_skyboxPixelShader, nullptr, 0);

    _skybox->Draw();

    // Present back buffer to screen
    _swapChain->Present(0, 0);
}