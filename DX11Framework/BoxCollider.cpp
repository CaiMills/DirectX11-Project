#include "BoxCollider.h"

bool BoxCollider::CollidesWith(BoxCollider& other)
{
    //AABB vs AABB Collision
    if (_min.x <= other._max.x && _max.x >= other._min.x &&
        _min.y <= other._max.y && _max.y >= other._min.y &&
        _min.z <= other._max.z && _max.z >= other._min.z)
    {
        DebugPrintF("AABB vs AABB Collision\n");
    }
    else
    {
        return false;
    }
}

bool BoxCollider::CollidesWith(SphereCollider& other)
{
    //AABB vs Sphere Collision
    
    return false;
}
