#include "BoxCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"

bool BoxCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    //// AABB vs AABB Collision
    Vector3 distance = GetPosition() - other.GetPosition();

    Vector3 boxAExtents = GetExtents();
    Vector3 boxBExtents = other.GetExtents();
    Vector3 halfExtents = boxAExtents + boxBExtents;

    if (abs(distance.x) <= halfExtents.x && abs(distance.y) <= halfExtents.y && abs(distance.z) <= halfExtents.z)
    {
        //Need to add Manifold Stuff
        return true;
    }

    return false;
}

bool BoxCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
    // AABB vs Sphere Collision
    return false;
}

bool BoxCollider::CollidesWith(PlaneCollider& other, CollisionManifold& out)
{
    // AABB vs Plane Collision
    return false;
}
