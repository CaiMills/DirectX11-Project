#pragma once
#include "GameObject.h"
#include "Camera.h"
#include <vector>

//#include <wrl.h>

using namespace DirectX;
//using Microsoft::WRL::ComPtr;

class DX11Framework
{
	int _WindowWidth = 1280;
	int _WindowHeight = 768;

	ID3D11DeviceContext* _immediateContext = nullptr;
	ID3D11Device* _device;
	IDXGIDevice* _dxgiDevice = nullptr;
	IDXGIFactory2* _dxgiFactory = nullptr;
	ID3D11RenderTargetView* _frameBufferView = nullptr;
	IDXGISwapChain1* _swapChain;
	D3D11_VIEWPORT _viewport;

	ID3D11RasterizerState* _fillState;
	ID3D11RasterizerState* _wireframeState;

	ID3D11InputLayout* _inputLayout;
	ID3D11VertexShader* _vertexShader;
	ID3D11VertexShader* _vertexShaderSkybox;
	ID3D11PixelShader* _pixelShader;
	ID3D11PixelShader* _pixelShaderSkybox;

	ID3D11Buffer* _constantBuffer;
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11DepthStencilState* _depthStencilSkybox;

	//World
	XMFLOAT4X4 _worldMatrix;
	XMFLOAT4X4 _skyboxMatrix;
	XMFLOAT4X4 _cubeWorldMatrix;
	XMFLOAT4X4 _lineWorldMatrix;
	XMFLOAT4X4 _pyramidWorldMatrix;
	XMFLOAT4X4 _starOBJWorldMatrix;
	XMFLOAT4X4 _donutOBJWorldMatrix;
	XMFLOAT4X4 _View;
	XMFLOAT4X4 _Projection;

	//Diffuse Lighting
	XMFLOAT4 _diffuseLight;
	XMFLOAT4 _diffuseMaterial;

	//Ambient Lighting
	XMFLOAT4 _ambientLight;
	XMFLOAT4 _ambientMaterial;

	//Specular Lighting
	XMFLOAT4 _specularLight;
	XMFLOAT4 _specularMaterial;
	XMFLOAT3 _cameraPosition;
	float _specPower;

	//Lighting
	XMFLOAT3 _lightDir;

	//Texturing
	int _hasTexture;
	int _hasSpecularMap;
	ID3D11SamplerState* _bilinearSamplerState;
	ID3D11ShaderResourceView* _skyboxTexture;
	ID3D11ShaderResourceView* _crateTexture;
	ID3D11ShaderResourceView* _asphaltTexture;

	HWND _windowHandle;

	//GameObjects
	//GameObject* _gameObject = new GameObject[10];
	MeshData donutOBJData;
	MeshData starOBJData;

	//Camera
	Camera* camera = new Camera[3];
	XMFLOAT4X4 _view;
	XMFLOAT4X4 _projection;
	int camNumber;
	XMFLOAT3 _operator;
	XMVECTOR _vector;
	XMVECTOR _operation;
	XMFLOAT3 _eyeMovement;
	XMVECTOR _eyeResult;
	XMFLOAT3 _lookAt;
	XMVECTOR _lookAtResult;


public:
	std::vector<gameObjectData> gameobjects; //Creates a list for all gameobjects
	ConstantBuffer _cbData;

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);
	HRESULT CreateWindowHandle(HINSTANCE hInstance, int nCmdShow);
	HRESULT CreateD3DDevice();
	HRESULT CreateSwapChainAndFrameBuffer();
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexIndexBuffers();
	HRESULT InitPipelineVariables();
	HRESULT InitRunTimeData();
	void Keybinds();
	~DX11Framework();
	void LoadLightingData();
	void LoadGameObjects();
	void Update();
	void Draw();
};