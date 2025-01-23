#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
	Vector3 _min = Vector3(1, 1, 1);
	Vector3 _max = Vector3(1, 1, 1);

public:
	BoxCollider(Transform* transform, Vector3 min, Vector3 max) : Collider(transform) { _min = min, _max = max; }

	virtual bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }
	virtual bool CollidesWith(BoxCollider& other) override;
	virtual bool CollidesWith(SphereCollider& other) override;
};

