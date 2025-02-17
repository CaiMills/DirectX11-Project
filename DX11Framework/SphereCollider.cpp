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
    // Sphere vs AABB Collision
    return false;
}

bool SphereCollider::CollidesWith(PlaneCollider& other, CollisionManifold& out)
{
    return false;
}
