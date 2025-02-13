#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
	float _radius = 1.0f;

public:
	SphereCollider(Transform* transform, Appearance* appearance, float radius) : Collider(transform, appearance) { _radius = radius; }

	virtual bool CollidesWith(Collider& other, CollisionManifold& out) override { return other.CollidesWith(*this, out); }
	virtual bool CollidesWith(SphereCollider& other, CollisionManifold& out) override;
	virtual bool CollidesWith(BoxCollider& other, CollisionManifold& out) override;
	virtual bool CollidesWith(PlaneCollider& other, CollisionManifold& out) override;

	virtual float GetRadius() override { return _radius; }
};

