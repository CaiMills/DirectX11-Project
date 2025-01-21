#include "SphereCollider.h"

bool SphereCollider::CollidesWith(SphereCollider& other)
{
	float combinedRadius = _radius + other._radius;
	Vector3 distanceFromTarget = this->GetPosition() - other.GetPosition();
	float distance = sqrt((distanceFromTarget.x * distanceFromTarget.x) + (distanceFromTarget.y * distanceFromTarget.y) + (distanceFromTarget.z * distanceFromTarget.z));
	if (distance < combinedRadius)
	{
		DebugPrintF("Collision\n");
		return true;
	}
	else
	{
		return false;
	}
}
