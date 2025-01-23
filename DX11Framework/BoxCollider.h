#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
	float _height = 0;
	float _width = 0;
	float _length = 0;

public:
	BoxCollider(Transform* transform) : Collider(transform) {};

	virtual bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }
	virtual bool CollidesWith(BoxCollider& other) override;
	virtual bool CollidesWith(SphereCollider& other) override;
};

