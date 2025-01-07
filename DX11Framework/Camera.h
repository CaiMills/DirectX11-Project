#pragma once
#include "Structures.h"

using namespace DirectX;

class Camera
{
private:
	XMFLOAT3 _eye;
	XMFLOAT3 _at;
	XMFLOAT3 _up;

	float _windowWidth;
	float _windowHeight;
	float _nearDepth;
	float _farDepth;

	XMFLOAT4X4 _view;
	XMFLOAT4X4 _projection;

	D3D11_VIEWPORT _viewPort;

public:
	Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~Camera();

	virtual void Update();

	XMFLOAT4X4 GetView() { return _view; }
	XMFLOAT4X4 GetProjection() { return _projection; }

	XMFLOAT3 GetEye() { return _eye; }
	XMFLOAT3 GetAt() { return _at; }
	XMFLOAT3 GetUp() { return _up; }

	void SetEye(XMFLOAT3 eye) { _eye = eye; }
	void SetAt(XMFLOAT3 at) { _at = at; }
	void SetUp(XMFLOAT3 up) { _up = up; }

	void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	XMFLOAT4X4 GetViewProjection() const;
};

