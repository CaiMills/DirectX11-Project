#include "BoxCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"

bool BoxCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    //// AABB vs AABB Collision
    Vector3 distance = GetPosition() - other.GetPosition();

    Vector3 objAExtents = GetAppearance()->GetMesh()->GetExtents();
    Vector3 objBExtents = other.GetAppearance()->GetMesh()->GetExtents();
    Vector3 halfExtents = (objAExtents / 2) + (objBExtents / 2);

    if (distance.x <= halfExtents.x && distance.y <= halfExtents.y && distance.z <= halfExtents.z)
    {
        //Need to add Manifold Stuff
        return true;
    }

    return false;
}

bool BoxCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
    // AABB vs Sphere Collision
    Vector3 boxMin = GetAppearance()->GetMesh()->GetMin();
    Vector3 boxMax = GetAppearance()->GetMesh()->GetMax();

    Vector3 clampedPoint;
    clampedPoint.x = max(boxMin.x, min(other.GetPosition().x, boxMax.x));
    clampedPoint.y = max(boxMin.y, min(other.GetPosition().y, boxMax.y));
    clampedPoint.z = max(boxMin.z, min(other.GetPosition().z, boxMax.z));

    Vector3 distance = clampedPoint - other.GetPosition();

    if (distance.Magnitude() < other.GetRadius())
    {
        return true;
    }

    return false;
}

bool BoxCollider::CollidesWith(PlaneCollider& other, CollisionManifold& out)
{
    return false;
}
