#include "SphereCollider.h"
#include "BoxCollider.h"
#include "PlaneCollider.h"

bool SphereCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
	// Sphere vs Sphere Collision
    Vector3 distance = GetPosition() - other.GetPosition();
    float radiiSum = GetRadius() + other.GetRadius();

    if (distance.Magnitude() < radiiSum)
	{
        out.collisionNormal = distance;
        out.collisionNormal.Normalize();
        out.contactPointCount = 1;
        out.points[0].Position = GetPosition() + (out.collisionNormal * GetRadius());
        out.points[0].penetrationDepth = fabs(distance.Magnitude() - radiiSum);
		return true;
	}
    return false;
}

bool SphereCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    //// Sphere vs AABB Collision (OUTDATED)
    //Vector3 closestPoint;
    //closestPoint.x = max(other.GetAppearance()->GetMesh()->GetMin().x, min(GetPosition().x, other.GetAppearance()->GetMesh()->GetMax().x));
    //closestPoint.y = max(other.GetAppearance()->GetMesh()->GetMin().y, min(GetPosition().y, other.GetAppearance()->GetMesh()->GetMax().y));
    //closestPoint.z = max(other.GetAppearance()->GetMesh()->GetMin().z, min(GetPosition().z, other.GetAppearance()->GetMesh()->GetMax().z));

    //float distance = sqrt(pow(closestPoint.x - GetPosition().x, 2) + 
    //    pow(closestPoint.y - GetPosition().y, 2) + 
    //    pow(closestPoint.z - GetPosition().z, 2));

    //if (distance < GetRadius())
    //{
    //    DebugPrintF("Sphere vs AABB Collision\n");
    //    return true;
    //}
    //else
    //{
    //    return false;
    //}

    return false;
}

bool SphereCollider::CollidesWith(PlaneCollider& other, CollisionManifold& out)
{
    return false;
}
