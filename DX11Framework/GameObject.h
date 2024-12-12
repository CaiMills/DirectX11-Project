#pragma once
#include "Structures.h"
#include "Appearance.h"

class GameObject
{
private:
	Appearance* _appearance;
	XMFLOAT3 _position = XMFLOAT3();
	XMFLOAT3 _rotation = XMFLOAT3();
	XMFLOAT3 _scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMFLOAT4X4 _world;

public:
	GameObject();
	~GameObject();

	Appearance* GetAppearance() { return _appearance; }
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void SetAppearance(Appearance* in) { _appearance = in; }

	void SetScale(XMFLOAT3 scale) { _scale = scale; }
	void SetRotation(XMFLOAT3 rotation) { _rotation = rotation; }
	void SetPosition(XMFLOAT3 position) { _position = position; }

	XMFLOAT3 GetPosition() const { return _position; }
	XMFLOAT3 GetScale() const { return _scale; }
	XMFLOAT3 GetRotation() const { return _rotation; }

	void Update(float deltaTime);

	void Draw(ID3D11DeviceContext* _immediateContext);
};

