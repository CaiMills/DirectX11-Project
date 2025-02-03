#pragma once
#include "Collider.h"
#include "Appearance.h"

class BoxCollider : public Collider
{
	Appearance* _appearance;

public:
	BoxCollider(Transform* transform, Appearance* appearance) : Collider(transform) { _appearance = appearance; }

	virtual bool CollidesWith(Collider& other, CollisionManifold& out) override { return other.CollidesWith(*this, out); }
	virtual bool CollidesWith(BoxCollider& other, CollisionManifold& out) override;
	virtual bool CollidesWith(SphereCollider& other, CollisionManifold& out) override;
	virtual bool CollidesWith(PlaneCollider& other, CollisionManifold& out) override;

	Appearance* GetAppearance() const { return _appearance; }
};


