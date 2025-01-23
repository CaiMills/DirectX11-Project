#include "BoxCollider.h"

bool BoxCollider::CollidesWith(BoxCollider& other)
{
    //AABB vs AABB Collision
    return false;
}

bool BoxCollider::CollidesWith(SphereCollider& other)
{
    //AABB vs Sphere Collision
    return false;
}
