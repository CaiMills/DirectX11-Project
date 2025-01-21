#include "SphereCollider.h"

bool SphereCollider::CollidesWith(SphereCollider& other)
{
	float combinedRadius = _radius + other._radius;
	Vector3 distanceFromTarget = this->GetPosition() - other.GetPosition();
	
	//Magnitude Calculation
	float distance = distanceFromTarget.Magnitude();

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
