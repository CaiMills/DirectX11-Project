#pragma once
#include "PhysicsModel.h"

class StaticBody : public PhysicsModel
{
public:
	StaticBody(Transform* transform) : PhysicsModel(transform) {};

	void SetMass(float mass) override { _mass = mass; };
	virtual void AddRelativeForce(Vector3 force, Vector3 point) override { AddForce(force); }

	virtual void Update(float deltaTime) override;
};

