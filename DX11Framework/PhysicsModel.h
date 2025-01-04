#pragma once
#include "Transform.h"

class PhysicsModel
{
protected:
	Transform* _transform;
	Vector3 _velocity;
	Vector3 _acceleration;
	bool _isAccelerate;

public:
	PhysicsModel(Transform* transform);

	void Update(float deltaTime);

	void SetVelocity(Vector3 velocity, bool isAccelerate) { _velocity = velocity, _isAccelerate = _isAccelerate; }
	Vector3 GetVelocity() const { return _velocity; }
};

