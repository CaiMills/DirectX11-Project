#include "PlaneCollider.h"

bool PlaneCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
    return false;
}

bool PlaneCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    return false;
}
