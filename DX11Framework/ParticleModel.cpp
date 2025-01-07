#include "ParticleModel.h"

void ParticleModel::Update(float deltaTime)
{
	timeAlive += deltaTime;
	if (timeAlive > resetTime)
	{
		Reset();
	}

	PhysicsModel::Update(deltaTime);
}

void ParticleModel::Reset()
{
	timeAlive = 0;
}
