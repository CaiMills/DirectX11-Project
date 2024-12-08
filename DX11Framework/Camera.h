#pragma once
#include "Structures.h"

using namespace DirectX;

class Camera
{
private:
	// Private attributes to store the camera position and view volume
	XMFLOAT3 _eye;
	XMFLOAT3 _at;
	XMFLOAT3 _up;

	float _windowWidth = 1208;
	float _windowHeight = 768;
	float _aspect = _windowWidth / _windowHeight;

	float _nearDepth;
	float _farDepth;

	// attributes to hold the view and projection matrices which will be passed to the shader

	XMFLOAT4X4 _view;
	XMFLOAT4X4 _projection;

	D3D11_VIEWPORT _viewPort;

public:
	Camera();
	~Camera();

	virtual void Update();

	XMFLOAT3 GetEye() { return _eye; }
	void SetEye(XMFLOAT3 eye) { _eye = eye; }

	XMFLOAT3 GetAt() { return _at; }
	void SetAt(XMFLOAT3 at) { _at = at; }

	XMFLOAT3 GetUp() { return _up; }
	void SetUp(XMFLOAT3 up) { _up = up; }

	D3D11_VIEWPORT GetViewPort() { return _viewPort; }

	XMFLOAT4X4 GetView() { return _view; }
	XMFLOAT4X4 GetProjection() { return _projection; }
};

