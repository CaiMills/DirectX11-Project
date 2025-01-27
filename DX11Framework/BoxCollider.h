#pragma once
#include "Collider.h"
#include "Appearance.h"

class BoxCollider : public Collider
{
	Appearance* _appearance;

public:
	BoxCollider(Transform* transform, Appearance* appearance) : Collider(transform) { _appearance = appearance; }

	virtual bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }
	virtual bool CollidesWith(BoxCollider& other) override;
	virtual bool CollidesWith(SphereCollider& other) override;

	Appearance* GetAppearance() { return _appearance; }
};


