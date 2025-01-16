#include "SphereCollider.h"

bool SphereCollider::CollidesWith(SphereCollider& other)
{
	float combinedRadius = this->GetRadius() + other.GetRadius();
	Vector3 collisionNormal = Vector3(other.GetPosition().x / other.GetPosition().Magnitude(), other.GetPosition().y / other.GetPosition().Magnitude(), other.GetPosition().z / other.GetPosition().Magnitude());
	if (collisionNormal.x <= combinedRadius || collisionNormal.y <= combinedRadius || collisionNormal.z <= combinedRadius)
	{
		return true;
	}
	else { return false; }
}
