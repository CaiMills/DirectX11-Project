#include "GameObject.h"

GameObject::GameObject()
{
	_transform = new Transform();
	_physicsModel = new PhysicsModel(_transform);
	_world = XMFLOAT4X4();
}

GameObject::~GameObject()
{
	_appearance = nullptr;
	_transform = nullptr;
	_physicsModel = nullptr;
}

void GameObject::Update(float deltaTime)
{
	if (_transform != nullptr)
	{
		_physicsModel->Update(deltaTime);
	}

	static float rotationAngle = 0.0f; // Initialize once

	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_transform->GetScale().x, _transform->GetScale().y, _transform->GetScale().z);
	XMMATRIX rotation = XMMatrixRotationX((rotationAngle += deltaTime * _transform->GetRotation().x)) * XMMatrixRotationY((rotationAngle += deltaTime * _transform->GetRotation().y)) * XMMatrixRotationZ((rotationAngle += deltaTime * _transform->GetRotation().z));
	XMMATRIX translation = XMMatrixTranslation(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

	XMStoreFloat4x4(&_world, XMMatrixIdentity() * scale * rotation * translation);
}

void GameObject::Draw(ID3D11DeviceContext* _immediateContext)
{
	_appearance->Draw(_immediateContext);
}