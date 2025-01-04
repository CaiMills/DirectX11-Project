#pragma once
#include "Structures.h"
#include "Appearance.h"
#include "PhysicsModel.h"
#include "Transform.h"

class GameObject
{
private:
	Appearance* _appearance = nullptr;
	Transform* _transform = nullptr;
	PhysicsModel* _physicsModel = nullptr;
	XMFLOAT4X4 _world;

public:
	GameObject();
	~GameObject();

	Appearance* GetAppearance() { return _appearance; }
	Transform* GetTransform() { return _transform; }
	PhysicsModel* GetPhysicsModel() const { return _physicsModel; }
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void SetAppearance(Appearance* in) { _appearance = in; }

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* _immediateContext);
};

