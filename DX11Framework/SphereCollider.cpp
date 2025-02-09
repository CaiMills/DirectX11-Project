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
    // DOESNT WORK CURRENTLY
    // Sphere vs AABB Collision
    Vector3 boxMin = other.GetAppearance()->GetMesh()->GetMin();
    Vector3 boxMax = other.GetAppearance()->GetMesh()->GetMax();

    Vector3 boxExtents = other.GetAppearance()->GetMesh()->GetExtents();
    Vector3 halfExtents = boxExtents / 2;

    Vector3 clampedPoint;
    clampedPoint.x = max(boxMin.x, min(GetPosition().x, boxMax.x));
    clampedPoint.y = max(boxMin.y, min(GetPosition().y, boxMax.y));
    clampedPoint.z = max(boxMin.z, min(GetPosition().z, boxMax.z));

    Vector3 distance = clampedPoint - GetPosition();

    if (distance.Magnitude() <= GetRadius())
    {
        DebugPrintF("Inside Circle");
        return true;
    }

    return false;
}

bool SphereCollider::CollidesWith(PlaneCollider& other, CollisionManifold& out)
{
    return false;
}
