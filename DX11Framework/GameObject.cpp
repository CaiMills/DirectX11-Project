#include "GameObject.h"

GameObject::GameObject()
{
	_transform = new Transform();
}

GameObject::~GameObject()
{
	_appearance = nullptr;
	_transform = nullptr;
}

void GameObject::Update(float deltaTime)
{
	static float rotationAngle = 0.0f; // Initialize once
	rotationAngle += (deltaTime * 1.0f); // Adjust rotation speed as needed

    // Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(GetTransform()->GetScale().x, GetTransform()->GetScale().y, GetTransform()->GetScale().z);
	XMMATRIX rotation = XMMatrixRotationX(rotationAngle) * XMMatrixRotationY(rotationAngle) * XMMatrixRotationZ(GetTransform()->GetRotation().z);
	XMMATRIX translation = XMMatrixTranslation(GetTransform()->GetPosition().x, GetTransform()->GetPosition().y, GetTransform()->GetPosition().z);

	XMStoreFloat4x4(&_world, XMMatrixIdentity() * scale * rotation * translation);
}

void GameObject::Draw(ID3D11DeviceContext* _immediateContext)
{
	_appearance->Draw(_immediateContext);
}