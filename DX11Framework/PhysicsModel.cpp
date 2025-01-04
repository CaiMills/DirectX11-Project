#include "PhysicsModel.h"

PhysicsModel::PhysicsModel(Transform* transform)
{
	_transform = transform;
	_isAccelerate = false;
}

void PhysicsModel::Update(float deltaTime)
{
	Vector3 position = _transform->GetPosition();

	if (_isAccelerate)
	{
		_velocity += _acceleration * deltaTime;
	}

	position += _velocity * deltaTime;
	_transform->SetPosition(position);
}
