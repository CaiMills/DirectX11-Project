#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
	XMFLOAT3 _min = XMFLOAT3();
	XMFLOAT3 _max = XMFLOAT3();

public:
	BoxCollider(Transform* transform, XMFLOAT3 min, XMFLOAT3 max) : Collider(transform) { _min = min, _max = max; }

	virtual bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }
	virtual bool CollidesWith(BoxCollider& other) override;
	virtual bool CollidesWith(SphereCollider& other) override;
};

