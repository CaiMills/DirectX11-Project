#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(Transform* transform) : Collider(transform) {};

	virtual bool CollidesWith(Collider& other, CollisionManifold& out) override { return other.CollidesWith(*this, out); }
	virtual bool CollidesWith(BoxCollider& other, CollisionManifold& out) override;
	virtual bool CollidesWith(SphereCollider& other, CollisionManifold& out) override;
	virtual bool CollidesWith(PlaneCollider& other, CollisionManifold& out) override;

	virtual float GetRadius() override { return 0.0f; }
};


