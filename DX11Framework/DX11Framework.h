#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "Camera.h"
#include "Timer.h"
#include "Mesh.h"

//#include <wrl.h>

using namespace DirectX;
//using Microsoft::WRL::ComPtr;

class DX11Framework
{
	// 2K
	//int _WindowWidth = 2560;
	//int _WindowHeight = 1440;

	// 1080p
	//int _WindowWidth = 1920;
	//int _WindowHeight = 1080;

	// 768p
	int _WindowWidth = 1280;
	int _WindowHeight = 768;

	HWND _windowHandle;

	IDXGIDevice* _dxgiDevice = nullptr;
	IDXGIFactory2* _dxgiFactory = nullptr;
	ID3D11RenderTargetView* _frameBufferView = nullptr;
	IDXGISwapChain1* _swapChain;
	D3D11_VIEWPORT _viewport;

	ID3D11DepthStencilState* _DSLessEqual;
	ID3D11RasterizerState* _fillState;
	ID3D11RasterizerState* _wireframeState;

	ID3D11InputLayout* _inputLayout;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;

	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;

	// World
	XMFLOAT4X4 _worldMatrix;
	XMFLOAT4X4 _View;
	XMFLOAT4X4 _Projection;

	// Diffuse Lighting
	XMFLOAT4 _diffuseLight;
	XMFLOAT4 _diffuseMaterial;

	// Ambient Lighting
	XMFLOAT4 _ambientLight;
	XMFLOAT4 _ambientMaterial;

	// Specular Lighting
	XMFLOAT4 _specularLight;
	XMFLOAT4 _specularMaterial;
	XMFLOAT3 _cameraPosition;
	float _specPower;

	// Lighting
	XMFLOAT3 _lightDir;
	
	// Timer
	Timer* _timer;

	// Texturing
	int _hasTexture;
	int _hasSpecularMap;
	ID3D11SamplerState* _bilinearSamplerState;



	// Skybox
	ID3D11InputLayout* _skyboxInputLayout;
	ID3D11VertexShader* _skyboxVertexShader;
	ID3D11PixelShader* _skyboxPixelShader;
	ID3D11DepthStencilState* _skyboxDepthStencil;
	XMFLOAT4X4 _skyboxMatrix;

	// GameObjects
	GameObject* _skybox = new GameObject();
	GameObject* _floor = new GameObject();
	GameObject* _cubes = new GameObject[4]();
	std::vector<gameObjectData> _gameObjectDataList;
	vector<GameObject*> _gameObjects;
	GameObject* _gameObject = new GameObject[10];

	// Camera
	Camera* _camera = nullptr;
	float _cameraOrbitRadius = 7.0f;
	float _cameraOrbitRadiusMin = 2.0f;
	float _cameraOrbitRadiusMax = 50.0f;
	float _cameraOrbitAngleXZ = -90.0f;
	float _cameraSpeed = 2.0f;

private:
	HRESULT CreateWindowHandle(HINSTANCE hInstance, int nCmdShow);
	HRESULT CreateD3DDevice();
	HRESULT CreateSwapChainAndFrameBuffer();
	HRESULT InitShadersAndInputLayout();
	HRESULT InitPipelineVariables();
	HRESULT InitRunTimeData();

public:
	~DX11Framework();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void RendererUpdates(float deltaTime);
	void PhysicsUpdates(float deltaTime);
	void Keybinds();
	void InitLighting();
	void InitGameObjects();
	void Update();
	void CollisionManager();
	void ResolveCollisions(GameObject* obj1, GameObject* obj2);
	void Draw();
};