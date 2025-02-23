#include "Transform.h"

Transform::Transform()
{
	_position = Vector3();
	_orientation = MakeQFromEulerAngles(0, 0, 0);
	_scale = Vector3(1.0f, 1.0f, 1.0f);
	_world = XMFLOAT4X4();
}

Transform::~Transform()
{
}

void Transform::Update(float deltaTime)
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);

	Quaternion quaternion = GetOrientation();
	XMVECTOR quaternionVector = XMVectorSet(quaternion.v.x, quaternion.v.y, quaternion.v.z, quaternion.n);
	XMMATRIX orientation = XMMatrixRotationQuaternion(quaternionVector);

	XMMATRIX position = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);

	XMStoreFloat4x4(&_world, XMMatrixIdentity() * scale * orientation * position);
}

void Transform::Move(Vector3 direction)
{
	_position.x += direction.x;
	_position.y += direction.y;
	_position.z += direction.z;
}
