#pragma once
#include "Structures.h"
#include "Appearance.h"
#include "Transform.h"

class GameObject
{
private:
	Appearance* _appearance;
	Transform* _transform = new Transform();
	XMFLOAT4X4 _world;

public:
	GameObject();
	~GameObject();

	Appearance* GetAppearance() { return _appearance; }
	Transform* GetTransform() { return _transform; }

	void SetAppearance(Appearance* in) { _appearance = in; }
	void SetTransform(Transform* in) { _transform = in; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* _immediateContext);
};

