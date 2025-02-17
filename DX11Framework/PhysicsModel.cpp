#include "PhysicsModel.h"

PhysicsModel::PhysicsModel(Transform* transform)
{
	_transform = transform;
}

PhysicsModel::~PhysicsModel()
{
	_transform = nullptr;
	_collider = nullptr;
}

void PhysicsModel::DragForce()
{
	float width = _transform->GetScale().x;
	float length = _transform->GetScale().y;
	float crossSection = width * length;

	float dragCoeffient;
	float radius = GetCollider()->GetRadius();

	// Circle Collider
	if (radius > 0)
	{
		dragCoeffient = 1.05f;
	}
	// Box Collider
	else
	{
		dragCoeffient = 0.47f;
	}
	_drag = dragCoeffient * crossSection * 0.5 * radius * (_velocity * _velocity);
}

void PhysicsModel::FrictionForce()
{
	float coefficient = 0.6f;

	_frictionForce = coefficient * _netForce.Magnitude();
}

void PhysicsModel::Update(float deltaTime)
{
	Vector3 position = _transform->GetPosition();

	if (_mass == 0)
	{
		return;
	}
	if (_simulateGravity)
	{
		_netForce += GravityForce();
	}
	if (!_constVelocity)
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
