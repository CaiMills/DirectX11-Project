#include "SphereCollider.h"

bool SphereCollider::CollidesWith(SphereCollider& other)
{
	float combinedRadius = _radius + other._radius;
	Vector3 distanceFromTarget = this->GetPosition() - other.GetPosition();
	
	//Magnitude Calculation
	float distance = sqrt((distanceFromTarget.x * distanceFromTarget.x) + (distanceFromTarget.y * distanceFromTarget.y) + (distanceFromTarget.z * distanceFromTarget.z));

	if (distance < combinedRadius)
	{

		return true;
	}
	else
	{
		return false;
	}
}
