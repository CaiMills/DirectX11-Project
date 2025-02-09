#include "BoxCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"

bool BoxCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    Vector3 distance = GetPosition() - other.GetPosition();

    Vector3 objAExtents = GetAppearance()->GetMesh()->GetExtents();
    Vector3 objBExtents = other.GetAppearance()->GetMesh()->GetExtents();
    Vector3 halfExtents = objAExtents / 2 + objBExtents / 2;

    if (distance.x <= halfExtents.x && distance.y <= halfExtents.y && distance.z <= halfExtents.z)
    {
        return true;
    }

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
