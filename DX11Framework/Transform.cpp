#include "Transform.h"

Transform::Transform()
{
	_position = Vector3();
	_rotation = Vector3();
	_scale = Vector3(1.0f, 1.0f, 1.0f);
}

Transform::~Transform()
{
}

void Transform::Move(Vector3 direction)
{
	_position.x += direction.x;
	_position.y += direction.y;
	_position.z += direction.z;
}
