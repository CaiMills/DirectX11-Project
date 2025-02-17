#include "BoxCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"

bool BoxCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    //// AABB vs AABB Collision
    Vector3 distance = GetPosition() - other.GetPosition();

    Vector3 boxAExtents = GetPosition();
    Vector3 boxBExtents = other.GetPosition();
    Vector3 halfExtents = (boxAExtents / 2) + (boxBExtents / 2);

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
    return false;
}
