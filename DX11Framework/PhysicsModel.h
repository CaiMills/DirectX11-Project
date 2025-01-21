#pragma once
#include "Collider.h"
#include "SphereCollider.h"

class PhysicsModel abstract
{
protected:
	Transform* _transform;
	Collider* _collider;
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

	bool IsCollideable() const { return _collider != nullptr; }
	void SetCollider(Collider* collider) { _collider = collider; }
	Collider* GetCollider() const { return _collider; }

	Vector3 GravityForce();
	void SetVelocity(Vector3 velocity, bool constVelocity) { _velocity = velocity, _constVelocity = constVelocity; }
	Vector3 GetVelocity() const { return _velocity; }
	void SetAcceleration(Vector3 acceleration, bool constAccelerate) { _acceleration = acceleration, _constAccelerate = constAccelerate; }
	Vector3 GetAcceleration() const { return _acceleration; }
	float GetMass() const { return _mass; }

	virtual void AddForce(Vector3 force) { _netForce += force; }
	virtual void ApplyImpulse(Vector3 impulse) { _velocity += impulse; }

	virtual void Update(float deltaTime);
};

