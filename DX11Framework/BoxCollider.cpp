#include "BoxCollider.h"

bool BoxCollider::CollidesWith(BoxCollider& other)
{
    //AABB vs AABB Collision
    if (_appearance->GetMin().x <= other.GetAppearance()->GetMax().x && _appearance->GetMax().x >= other.GetAppearance()->GetMin().x &&
        _appearance->GetMin().y <= other.GetAppearance()->GetMax().y && _appearance->GetMax().y >= other.GetAppearance()->GetMin().y &&
        _appearance->GetMin().z <= other.GetAppearance()->GetMax().z && _appearance->GetMax().z >= other.GetAppearance()->GetMin().z)
    {
        DebugPrintF("AABB vs AABB Collision\n");
        return true;
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
