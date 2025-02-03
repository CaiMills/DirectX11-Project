#include "BoxCollider.h"
#include "SphereCollider.h"

bool BoxCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    //// AABB vs AABB Collision (OUTDATED)
    //if (_appearance->GetMesh()->GetMin().x <= other.GetAppearance()->GetMesh()->GetMax().x && _appearance->GetMesh()->GetMax().x >= other.GetAppearance()->GetMesh()->GetMin().x &&
    //    _appearance->GetMesh()->GetMin().y <= other.GetAppearance()->GetMesh()->GetMax().y && _appearance->GetMesh()->GetMax().y >= other.GetAppearance()->GetMesh()->GetMin().y &&
    //    _appearance->GetMesh()->GetMin().z <= other.GetAppearance()->GetMesh()->GetMax().z && _appearance->GetMesh()->GetMax().z >= other.GetAppearance()->GetMesh()->GetMin().z)
    //{
    //    DebugPrintF("AABB vs AABB Collision\n");
    //    return true;
    //}
    //else
    //{
    //    return false;
    //}

    return false;
}

bool BoxCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
    //// AABB vs Sphere Collision (OUTDATED)
    //Vector3 closestPoint;
    //closestPoint.x = max(_appearance->GetMesh()->GetMin().x, min(other.GetPosition().x, _appearance->GetMesh()->GetMax().x));
    //closestPoint.y = max(_appearance->GetMesh()->GetMin().y, min(other.GetPosition().y, _appearance->GetMesh()->GetMax().y));
    //closestPoint.z = max(_appearance->GetMesh()->GetMin().z, min(other.GetPosition().z, _appearance->GetMesh()->GetMax().z));

    //float distance = sqrt(pow(closestPoint.x - other.GetPosition().x, 2) + 
    //    pow(closestPoint.y - other.GetPosition().y, 2) + 
    //    pow(closestPoint.z - other.GetPosition().z, 2));

    //if (distance < other.GetRadius())
    //{
    //    DebugPrintF("AABB vs Sphere Collision\n");
    //    return true;
    //}
    //else
    //{
    //    return false;
    //}

    return false;
}

bool BoxCollider::CollidesWith(PlaneCollider& other, CollisionManifold& out)
{
    return false;
}
