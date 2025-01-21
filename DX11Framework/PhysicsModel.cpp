#include "PhysicsModel.h"

PhysicsModel::PhysicsModel(Transform* transform)
{
	_transform = transform;
}

PhysicsModel::~PhysicsModel()
{
	_transform = nullptr;
	_collider = nullptr;

	_velocity = Vector3();
	_acceleration = Vector3();
	_constVelocity = false;
	_constAccelerate = false;
}

Vector3 PhysicsModel::GravityForce()
{
	float gravity = -9.81f * _mass;
	return Vector3(0, gravity, 0);
}

void PhysicsModel::Update(float deltaTime)
{
	Vector3 position = _transform->GetPosition();

	if (_simulateGravity)
	{
		_netForce += GravityForce();
	}
	if (_constVelocity)
	{
		position += _velocity * deltaTime;
	}
	if (_constAccelerate)
	{
		_velocity += _acceleration * deltaTime;
		position += _velocity * deltaTime;
	}

	_acceleration += _netForce / _mass;
	_velocity += _acceleration * deltaTime;
	position += _velocity * deltaTime;
	_netForce = Vector3();
	_acceleration = Vector3();

	_transform->SetPosition(position);
}
