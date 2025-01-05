#include "DX11Framework.h"
#include "Line.h"
#include "Cube.h"
#include "Pyramid.h"
#include <atlstr.h> // to use CString.

//#define RETURNFAIL(x) if(FAILED(x)) return x;

Cube _skybox;

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

    //other shader
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

    ///////////////////////////////////////////////////////////////////////////////////////////////

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

    _skybox.VertexData(_device);
    _skybox.IndexData(_device, true);

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

    return S_OK;
}

HRESULT DX11Framework::InitRunTimeData()
{
    HRESULT hr = S_OK;

    //Camera Settings - This is where they are initalised
    camNumber = 0;

    _view = _camera[0].GetView();
    _projection = _camera[0].GetProjection();

    _camera[0].SetEye(XMFLOAT3(0, 0, -6.1));
    _camera[0].SetAt(XMFLOAT3(0, 0, 0));
    _camera[0].SetUp(XMFLOAT3(0, 1, 0));

    _camera[1].SetEye(XMFLOAT3(0, 0, -20.1));
    _camera[1].SetAt(XMFLOAT3(0, 0, 0));
    _camera[1].SetUp(XMFLOAT3(0, 1, 0));

    _camera[2].SetEye(XMFLOAT3(0, 0, -6.1));
    _camera[2].SetAt(XMFLOAT3(0, 0, 0));
    _camera[2].SetUp(XMFLOAT3(0, 1, 0));

    //Skybox
    hr = CreateDDSTextureFromFile(_device, L"Textures\\Free Assets Online\\spyro3Skybox.dds", nullptr, &_skyboxTexture);
    _skybox.SetTexture(_skyboxTexture);

    //Game Objects
    LoadGameObjects();

    if (FAILED(hr))
    {
        return hr;
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

#pragma region GOMovementControls
    //NUMPAD 8 - Fowards
    if (GetAsyncKeyState(0x68) & 0X0001)
    {
        _gameObject[0].GetTransform()->Move(Vector3(0, 0, 0.1f));
        _gameObject[1].GetTransform()->Move(Vector3(0, 0, 0.1f));
    }
    //NUMPAD 2 - Backwards
    if (GetAsyncKeyState(0x62) & 0X0001)
    {
        _gameObject[0].GetTransform()->Move(Vector3(0, 0, -0.1f));
        _gameObject[1].GetTransform()->Move(Vector3(0, 0, -0.1f));
    }
    //NUMPAD 4 - Left
    if (GetAsyncKeyState(0x64) & 0X0001)
    {
        _gameObject[0].GetTransform()->Move(Vector3(-0.1f, 0, 0));
        _gameObject[1].GetTransform()->Move(Vector3(-0.1f, 0, 0));
    }
    //NUMPAD 6 - Right
    if (GetAsyncKeyState(0x66) & 0X0001)
    {
        _gameObject[0].GetTransform()->Move(Vector3(0.1f, 0, 0));
        _gameObject[1].GetTransform()->Move(Vector3(0.1f, 0, 0));
    }
    //NUMPAD 9 - Up
    if (GetAsyncKeyState(0x69) & 0X0001)
    {
        _gameObject[0].GetTransform()->Move(Vector3(0, 0.1f, 0));
        _gameObject[1].GetTransform()->Move(Vector3(0, 0.1f, 0));
    }
    //NUMPAD 3 - Down
    if (GetAsyncKeyState(0x63) & 0X0001)
    {
        _gameObject[0].GetTransform()->Move(Vector3(0, -0.1f, 0));
        _gameObject[1].GetTransform()->Move(Vector3(0, -0.1f, 0));
    }
#pragma endregion

#pragma region GOVelocityControls
    //UP ARROW - Fowards Constant Velocity
    if (GetAsyncKeyState(0x26) & 0X0001)
    {
        _gameObject[0].GetPhysicsModel()->SetVelocity(Vector3(0, 0, 1), true);
        _gameObject[1].GetPhysicsModel()->SetVelocity(Vector3(0, 0, 1), false);
    }
    //DOWN ARROW - Backwards Constant Velocity
    if (GetAsyncKeyState(0x28) & 0X0001)
    {
        _gameObject[0].GetPhysicsModel()->SetVelocity(Vector3(0, 0, -1), true);
        _gameObject[1].GetPhysicsModel()->SetVelocity(Vector3(0, 0, -1), false);
    }
    //LEFT ARROW - Left Constant Velocity
    if (GetAsyncKeyState(0x25) & 0X0001)
    {
        _gameObject[0].GetPhysicsModel()->SetVelocity(Vector3(-1, 0, 0), true);
        _gameObject[1].GetPhysicsModel()->SetVelocity(Vector3(-1, 0, 0), false);
    }
    //RIGHT ARROW - Right Constant Velocity
    if (GetAsyncKeyState(0x27) & 0X0001)
    {
        _gameObject[0].GetPhysicsModel()->SetVelocity(Vector3(1, 0, 0), true);
        _gameObject[1].GetPhysicsModel()->SetVelocity(Vector3(1, 0, 0), false);
    }
    //PAGE UP - Up Constant Velocity
    if (GetAsyncKeyState(0x22) & 0X0001)
    {
        _gameObject[0].GetPhysicsModel()->SetVelocity(Vector3(0, -1, 0), true);
        _gameObject[1].GetPhysicsModel()->SetVelocity(Vector3(0, -1, 0), false);
    }
    //PAGE DOWN - Down Constant Velocity
    if (GetAsyncKeyState(0x21) & 0X0001)
    {
        _gameObject[0].GetPhysicsModel()->SetVelocity(Vector3(0, 1, 0), true);
        _gameObject[1].GetPhysicsModel()->SetVelocity(Vector3(0, 1, 0), false);
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
        _gameObject[0].GetPhysicsModel()->SetVelocity(Vector3(0, 0, 0), true);
        _gameObject[1].GetPhysicsModel()->SetVelocity(Vector3(0, 0, 0), false);
    }
#pragma endregion

#pragma region CameraSwitching
    //Switch Cameras
    //1 - Basic Static Camera
    if (GetAsyncKeyState(0x31) & 0x0001)
    {
        camNumber = 0;
    }
    //2 - Basic Static Camera 2
    if (GetAsyncKeyState(0x32) & 0x0001)
    {
        camNumber = 1;
    }
    //3 - FreeCam
    if (GetAsyncKeyState(0x33) & 0x0001)
    {
        camNumber = 2;
    }
#pragma endregion

#pragma region FreeCamMovement
    if (camNumber == 2)
    {
        //W - Fowards
        if (GetAsyncKeyState(0x57) & 0X0001)
        {
            _eyeMovement = _camera[2].GetEye();
            _operator = XMFLOAT3(0, 0, 1.0f);
            _vector = XMLoadFloat3(&_eyeMovement);
            _operation = XMLoadFloat3(&_operator);
            _eyeResult = XMVectorAdd(_vector, _operation);
            XMStoreFloat3(&_eyeMovement, _eyeResult);

            _lookAt = _camera[2].GetAt();
            _operator = XMFLOAT3(0, 0, 1.0f);
            _vector = XMLoadFloat3(&_lookAt);
            _operation = XMLoadFloat3(&_operator);
            _lookAtResult = XMVectorAdd(_vector, _operation);
            XMStoreFloat3(&_lookAt, _lookAtResult);

            _View = _camera[2].GetView();
            _Projection = _camera[2].GetProjection();

            _camera[2].SetEye(XMFLOAT3(_eyeMovement.x, _eyeMovement.y, _eyeMovement.z));
            _camera[2].SetAt(XMFLOAT3(_lookAt.x, _lookAt.y, _lookAt.z));
            return;
        }
        //S - Backwards
        if (GetAsyncKeyState(0x53) & 0X0001)
        {
            _eyeMovement = _camera[2].GetEye();
            _operator = XMFLOAT3(0, 0, -1.0f);
            _vector = XMLoadFloat3(&_eyeMovement);
            _operation = XMLoadFloat3(&_operator);
            _eyeResult = XMVectorAdd(_vector, _operation);
            XMStoreFloat3(&_eyeMovement, _eyeResult);

            _lookAt = _camera[2].GetAt();
            _operator = XMFLOAT3(0, 0, -1.0f);
            _vector = XMLoadFloat3(&_lookAt);
            _operation = XMLoadFloat3(&_operator);
            _lookAtResult = XMVectorAdd(_vector, _operation);
            XMStoreFloat3(&_lookAt, _lookAtResult);

            _View = _camera[2].GetView();
            _Projection = _camera[2].GetProjection();

            _camera[2].SetEye(XMFLOAT3(_eyeMovement.x, _eyeMovement.y, _eyeMovement.z));
            _camera[2].SetAt(XMFLOAT3(_lookAt.x, _lookAt.y, _lookAt.z));
            return;
        }
        //A - Left
        if (GetAsyncKeyState(0x41) & 0X0001)
        {
            //NEED TO ADD
        }
        //D - Left
        if (GetAsyncKeyState(0x44) & 0X0001)
        {
            //NEED TO ADD
        }
    }
    //Resets the FreeCam settings if FreeCam isnt active
    else if (camNumber != 2)
    {
        _camera[2].SetEye(XMFLOAT3(0, 0, -6.1));
        _camera[2].SetAt(XMFLOAT3(0, 0, 0));
        _camera[2].SetUp(XMFLOAT3(0, 1, 0));
    }
#pragma endregion
}

DX11Framework::~DX11Framework()
{
    //need to delete gameobjects and cameras
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

/// <summary>
/// A json loader that specifically loads the Game Object data
/// </summary>
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

        gameobjects.push_back(g); //Adds the gameobject to the list
    }

    for (int i = 0; i < gameobjects.size(); i++)
    {
        //mesh
        Appearance* _appearance = new Appearance();
        _appearance->SetMeshData(OBJLoader::Load(gameobjects.at(i).objFilePath, _device, false)); //pass the meshData into the GameObject Class

        //texture
        ID3D11ShaderResourceView* _texture;
        std::wstring colorTexFilePath = static_cast<CString>(gameobjects.at(i).specularTexture.c_str()).GetString(); //converts it to a wstring, so that it can be converted to a texture
        CreateDDSTextureFromFile(_device, colorTexFilePath.c_str(), nullptr, &_texture);

        std::wstring specTexFilePath = static_cast<CString>(gameobjects.at(i).colorTexture.c_str()).GetString();
        CreateDDSTextureFromFile(_device, specTexFilePath.c_str(), nullptr, &_texture);

        _appearance->SetTexture(_texture);
        _gameObject[i].SetAppearance(_appearance);

        //transform
        _gameObject[i].GetTransform()->SetScale(Vector3(gameobjects.at(i).scale.x, gameobjects.at(i).scale.y, gameobjects.at(i).scale.z));
        _gameObject[i].GetTransform()->SetRotation(Vector3(gameobjects.at(i).rotation.x, gameobjects.at(i).rotation.y, gameobjects.at(i).rotation.z));
        _gameObject[i].GetTransform()->SetPosition(Vector3(gameobjects.at(i).position.x, gameobjects.at(i).position.y, gameobjects.at(i).position.z));
    }
}

void DX11Framework::Update()
{
    //Sets the view and projection to the currently active camera
    _view = _camera[camNumber].GetView();
    _projection = _camera[camNumber].GetProjection();

    _cbData.View = XMMatrixTranspose(XMLoadFloat4x4(&_view));
    _cbData.Projection = XMMatrixTranspose(XMLoadFloat4x4(&_projection));

    //Cameras
    _camera[camNumber].Update();

    //Static initializes this value only once    
    static ULONGLONG frameStart = GetTickCount64();

    ULONGLONG frameNow = GetTickCount64();
    float deltaTime = (frameNow - frameStart) / 1000.0f;
    frameStart = frameNow;

    //Defines the world position
    XMStoreFloat4x4(&_worldMatrix, XMMatrixIdentity());

    //Lighting
    LoadLightingData();
    _cbData.cameraPosition = _camera[0].GetEye();
    _cbData.specPower = 10;
    _cbData.lightDir = XMFLOAT3(0, 0.0f, -1.0f);
    _cbData.hasTexture = _hasTexture;
    _cbData.hasSpecularMap = _hasSpecularMap;

    //Update objects
    for (int i = 0; i < gameobjects.size(); i++)
    {
        _gameObject[i].Update(deltaTime);
    }

    Keybinds();
}

void DX11Framework::Draw()
{    
    //Present unbinds render target, so rebind and clear at start of each frame
    float backgroundColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };  

        //Write constant buffer data onto GPU
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;

    _immediateContext->OMSetRenderTargets(1, &_frameBufferView, _depthStencilView);
    _immediateContext->ClearRenderTargetView(_frameBufferView, backgroundColor);
    _immediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

    //Vertex and Pixel Shader, Set Shader
    _immediateContext->VSSetShader(_vertexShader, nullptr, 0);
    _immediateContext->PSSetShader(_pixelShader, nullptr, 0);

    //Store this frames data in constant buffer struct
    _cbData.World = XMMatrixTranspose(XMLoadFloat4x4(&_worldMatrix));

    //Loads Game Objects
    for (int i = 0; i < gameobjects.size(); i++)
    {
        // Set texture
        if (_gameObject[i].GetAppearance()->HasTexture())
        {
            _immediateContext->PSSetShaderResources(0, 1, _gameObject[i].GetAppearance()->GetTexture());
            _cbData.hasTexture = 1.0f;
            _cbData.hasSpecularMap = 1.0f;
        }
        else
        {
            _cbData.hasTexture = 0.0f;
            _cbData.hasSpecularMap = 0.0f;
        }
        _cbData.World = XMMatrixTranspose(_gameObject[i].GetTransform()->GetWorldMatrix());
        _immediateContext->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
        _immediateContext->Unmap(_constantBuffer, 0);

        _gameObject[i].Draw(_immediateContext);
    }
    //Changes the Stencil State to the Skybox one
    _immediateContext->OMSetDepthStencilState(_depthStencilSkybox, 0);

    //Vertex and Pixel Shader, Set Shader
    _immediateContext->VSSetShader(_vertexShaderSkybox, nullptr, 0);
    _immediateContext->PSSetShader(_pixelShaderSkybox, nullptr, 0);

    _cbData.World = XMMatrixTranspose(XMLoadFloat4x4(&_worldMatrix));
    _immediateContext->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
    memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
    _immediateContext->Unmap(_constantBuffer, 0);

    _skybox.Draw(_immediateContext);
    
    //Present Backbuffer to screen
    _swapChain->Present(0, 0);
}