#include "SphereCollider.h"
#include "BoxCollider.h"

bool SphereCollider::CollidesWith(SphereCollider& other)
{
	// Sphere Collision ( it may need to be put into a sqrt())
	float distance = pow(GetPosition().x - other.GetPosition().x, 2) +
		pow(GetPosition().y - other.GetPosition().y, 2) +
		pow(GetPosition().z - other.GetPosition().z, 2);

	if (distance < GetRadius() + other.GetRadius())
	{
		DebugPrintF("Sphere vs Sphere Collision\n");
		return true;
	}
	else
	{
		return false;
	}
}

bool SphereCollider::CollidesWith(BoxCollider& other)
{
    // Sphere vs AABB Collision
    Vector3 closestPoint;
    closestPoint.x = max(other.GetAppearance()->GetMesh()->GetMin().x, min(GetPosition().x, other.GetAppearance()->GetMesh()->GetMax().x));
    closestPoint.y = max(other.GetAppearance()->GetMesh()->GetMin().y, min(GetPosition().y, other.GetAppearance()->GetMesh()->GetMax().y));
    closestPoint.z = max(other.GetAppearance()->GetMesh()->GetMin().z, min(GetPosition().z, other.GetAppearance()->GetMesh()->GetMax().z));

    float distance = sqrt(pow(closestPoint.x - GetPosition().x, 2) + 
        pow(closestPoint.y - GetPosition().y, 2) + 
        pow(closestPoint.z - GetPosition().z, 2));

    if (distance < GetRadius())
    {
        DebugPrintF("Sphere vs AABB Collision\n");
        return true;
    }
    else
    {
        return false;
    }

    return false;
}
