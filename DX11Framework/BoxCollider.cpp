#include "BoxCollider.h"

bool BoxCollider::CollidesWith(BoxCollider& other)
{
    //AABB vs AABB Collision
    if (this->_min.x <= other._min.x || this->_max.x <= other._max.x ||
        this->_min.y <= other._min.y || this->_max.y <= other._max.y || 
        this->_min.z <= other._min.z || this->_max.z <= other._max.z)
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
