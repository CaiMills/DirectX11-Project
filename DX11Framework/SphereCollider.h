#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
	float _radius = 1.0f;

public:
	SphereCollider(Transform* transform, float radius) : Collider(transform) { _radius = radius; }

	virtual bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }
	virtual bool CollidesWith(SphereCollider& other) override;
	virtual bool CollidesWith(BoxCollider& other) override;

	float GetRadius() const { return _radius; }
};

