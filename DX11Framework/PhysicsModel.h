#pragma once
#include "Transform.h"

class PhysicsModel
{
protected:
	Transform* _transform;
	float _mass;
	bool _simulateGravity;

	Vector3 _velocity;
	bool _constVelocity;
	Vector3 _acceleration;
	bool _constAccelerate;
	Vector3 _netForce;

public:
	PhysicsModel(Transform* transform);
	~PhysicsModel();

	Vector3 GravityForce();

	void SetVelocity(Vector3 velocity, bool constVelocity) { _velocity = velocity, _constVelocity = constVelocity; }
	Vector3 GetVelocity() const { return _velocity; }
	void SetAcceleration(Vector3 acceleration, bool constAccelerate) { _acceleration = acceleration, _constAccelerate = constAccelerate; }
	Vector3 GetAcceleration() const { return _acceleration; }

	void AddForce(Vector3 force) { _netForce += force; }

	void Update(float deltaTime);
};

