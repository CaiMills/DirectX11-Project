#include "Transform.h"

Transform::Transform()
{
	_position = Vector3();
	_rotation = Vector3();
	_scale = Vector3(1.0f, 1.0f, 1.0f);
	_world = XMFLOAT4X4();
}

Transform::~Transform()
{
}

void Transform::Update(float deltaTime)
{
	static float rotationAngle = 0.0f; // Initialize once

	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	XMMATRIX rotation = XMMatrixRotationX(GetRotation().x) * XMMatrixRotationY(GetRotation().y) * XMMatrixRotationZ(GetRotation().z);
	XMMATRIX position = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);

	XMStoreFloat4x4(&_world, XMMatrixIdentity() * scale * rotation * position);
}

void Transform::Move(Vector3 direction)
{
	_position.x += direction.x;
	_position.y += direction.y;
	_position.z += direction.z;
}
