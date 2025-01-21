#pragma once
#include "Transform.h"

//Foward Declaration
class SphereCollider;

class Collider abstract
{
protected:
	Transform* _transform;

public:
	Collider(Transform* transform) { _transform = transform; }

	virtual bool CollidesWith(Collider& other) = 0;
	virtual bool CollidesWith(SphereCollider& other) = 0;

	Vector3 GetPosition() const { return _transform->GetPosition(); }
};

