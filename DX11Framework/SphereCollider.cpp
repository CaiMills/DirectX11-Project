#include "SphereCollider.h"

bool SphereCollider::CollidesWith(SphereCollider& other)
{
	//AABB Calculation
	float distance = pow(this->GetPosition().x - other.GetPosition().x, 2) +
		pow(this->GetPosition().y - other.GetPosition().y, 2) +
		pow(this->GetPosition().z - other.GetPosition().z, 2);

	if (distance < this->GetRadius() + other.GetRadius())
	{
		DebugPrintF("Collision\n");
		return true;
	}
	else
	{
		return false;
	}
}
