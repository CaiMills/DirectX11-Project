#pragma once
#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "PlaneCollider.h"

class PhysicsModel abstract
{
protected:
	Transform* _transform;
	Collider* _collider = nullptr;
	float _mass = 0.0f;
	bool _simulateGravity = false;

	Vector3 _velocity = Vector3();
	bool _constVelocity = false;
	Vector3 _acceleration = Vector3();
	bool _constAccelerate = false;
	Vector3 _netForce = Vector3();

public:
	PhysicsModel(Transform* transform);
	~PhysicsModel();

	// Collider Functionality
	bool IsCollideable() const { return _collider != nullptr; }
	void SetCollider(Collider* collider) { _collider = collider; }
	Collider* GetCollider() const { return _collider; }

	// Mass Functionality
	virtual void SetMass(float mass) = 0;
	float GetMass() const { return _mass; }
	float GetInverseMass() { if (_mass == 0) return 0; return 1.0f / _mass; }

	// Set Forces
	Vector3 GravityForce() { return Vector3(0, -9.81f * _mass, 0); }
	void SetVelocity(Vector3 velocity, bool constVelocity) { _velocity = velocity, _constVelocity = constVelocity; }
	Vector3 GetVelocity() const { return _velocity; }
	void SetAcceleration(Vector3 acceleration, bool constAccelerate) { _acceleration = acceleration, _constAccelerate = constAccelerate; }
	Vector3 GetAcceleration() const { return _acceleration; }

	// Apply Forces
	virtual void AddForce(Vector3 force) { _netForce += force; }
	virtual void ApplyImpulse(Vector3 impulse) { _velocity += impulse; }
	virtual void AddRelativeForce(Vector3 force, Vector3 point) = 0;

	virtual void Update(float deltaTime);
};

