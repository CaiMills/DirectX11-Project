#pragma once
#include "Collider.h"
class PlaneCollider : public Collider
{
public:
	PlaneCollider(Transform* transform) : Collider(transform) {};

	virtual bool CollidesWith(Collider& other, CollisionManifold& out) override { return other.CollidesWith(*this, out); }
	virtual bool CollidesWith(SphereCollider& other, CollisionManifold& out) override;
	virtual bool CollidesWith(BoxCollider& other, CollisionManifold& out) override;
};

