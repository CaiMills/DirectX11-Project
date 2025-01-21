#include "Transform.h"

Transform::Transform()
{
	_position = Vector3();
	_orientation = MakeQFromEulerAngles(0, 0, 0);
	_scale = Vector3(1.0f, 1.0f, 1.0f);
	_world = XMFLOAT4X4();
	_continuousRotation = false;
}

Transform::~Transform()
{
}

void Transform::Update(float deltaTime)
{
	//if (_continuousRotation)
	//{
	//	//Constant Rotation
	//	static float rotationAngle = 0.0f;
	//	rotationAngle += deltaTime;

	//	// Calculate world matrix
	//	XMMATRIX scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	//	XMVECTOR rotation = XMVectorSet(rotationAngle, GetRotation().y, GetRotation().z, 1);
	//	XMMATRIX position = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);

	//	XMMATRIX orientation = XMMatrixRotationQuaternion(rotation);

	//	XMStoreFloat4x4(&_world, XMMatrixIdentity() * scale * orientation * position);
	//}
	//else
	//{
	//	// Calculate world matrix
	//	XMMATRIX scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	//	XMMATRIX orientation = XMMatrixRotationQuaternion(XMVectorSet(GetRotation().x, GetRotation().y, GetRotation().z, 1));
	//	XMMATRIX position = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);

	//	XMStoreFloat4x4(&_world, XMMatrixIdentity() * scale * orientation * position);
	//}

	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	XMMATRIX orientation = XMMatrixRotationQuaternion(XMVectorSet(GetRotation().x, GetRotation().y, GetRotation().z, 1));
	XMMATRIX position = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);

	XMStoreFloat4x4(&_world, XMMatrixIdentity() * scale * orientation * position);
}

void Transform::Move(Vector3 direction)
{
	_position.x += direction.x;
	_position.y += direction.y;
	_position.z += direction.z;
}
