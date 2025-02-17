#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

bool PlaneCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
    return false;
}


bool PlaneCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    // Plane vs AABB Collision
    Vector3 distance = GetPosition() - other.GetPosition();

    Vector3 boxAExtents = GetPosition() / 2;
    Vector3 boxBExtents = other.GetPosition() / 2;
    Vector3 halfExtents = (boxAExtents / 2) + (boxBExtents / 2);

    if (abs(distance.x) <= halfExtents.x && abs(distance.y) <= halfExtents.y && abs(distance.z) <= halfExtents.z)
    {
        //Need to add Manifold Stuff
        return true;
    }

    return false;
}