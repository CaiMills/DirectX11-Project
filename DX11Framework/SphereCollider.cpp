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
    ////// Sphere vs AABB Collision
    //Vector3 boxMin = GetAppearance()->GetMesh()->GetMin();
    //Vector3 boxMax = GetAppearance()->GetMesh()->GetMax();

    //Vector3 clampedPoint;
    //clampedPoint.x = max(boxMin.x, min(other.GetPosition().x, boxMax.x));
    //clampedPoint.y = max(boxMin.y, min(other.GetPosition().y, boxMax.y));
    //clampedPoint.z = max(boxMin.z, min(other.GetPosition().z, boxMax.z));

    //Vector3 distance = clampedPoint - other.GetPosition();

    //if (distance.Magnitude() < other.GetRadius())
    //{
    //    return true;
    //}

    return false;
}

bool SphereCollider::CollidesWith(PlaneCollider& other, CollisionManifold& out)
{
    return false;
}
