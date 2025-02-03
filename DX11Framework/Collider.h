#pragma once
#include "Transform.h"

//Foward Declaration
class SphereCollider;
class BoxCollider;
class PlaneCollider;

class Collider abstract
{
protected:
	Transform* _transform;

public:
	Collider(Transform* transform) { _transform = transform; }

	virtual bool CollidesWith(Collider& other, CollisionManifold& out) = 0;
	virtual bool CollidesWith(SphereCollider& other, CollisionManifold& out) = 0;
	virtual bool CollidesWith(BoxCollider& other, CollisionManifold& out) = 0;
	virtual bool CollidesWith(PlaneCollider& other, CollisionManifold& out) = 0;

	Vector3 GetPosition() const { return _transform->GetPosition(); }
};

