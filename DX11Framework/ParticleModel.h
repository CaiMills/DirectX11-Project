#pragma once
#include "PhysicsModel.h"

class ParticleModel : public PhysicsModel
{
private:
	float timeAlive = 0;
	float resetTime;

public:
	ParticleModel(Transform* transform) : PhysicsModel(transform) {};
	ParticleModel(Transform* transform, float resetTime, Vector3 pertubation, bool invertGravity);

	virtual void AddRelativeForce(Vector3 force, Vector3 point) override { AddForce(force); }

	virtual void Update(float deltaTime) override;

	void Reset();
};

