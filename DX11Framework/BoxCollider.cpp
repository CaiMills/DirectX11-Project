#include "BoxCollider.h"
#include "SphereCollider.h"

bool BoxCollider::CollidesWith(BoxCollider& other)
{
    //AABB vs AABB Collision
    if (_appearance->GetMin().x <= other.GetAppearance()->GetMax().x && _appearance->GetMax().x >= other.GetAppearance()->GetMin().x &&
        _appearance->GetMin().y <= other.GetAppearance()->GetMax().y && _appearance->GetMax().y >= other.GetAppearance()->GetMin().y &&
        _appearance->GetMin().z <= other.GetAppearance()->GetMax().z && _appearance->GetMax().z >= other.GetAppearance()->GetMin().z)
    {
        //DebugPrintF("AABB vs AABB Collision\n");
        DebugPrintF("This Object's Min x is %f, Min y is %f, Min z is %f\n", _appearance->GetMin().x, _appearance->GetMin().y, _appearance->GetMin().z);
        DebugPrintF("This Object's Max x is %f, Max y is %f, Max z is %f\n", _appearance->GetMax().x, _appearance->GetMax().y, _appearance->GetMax().z);
        DebugPrintF("The Other Object's Min x is %f, Min y is %f, Min z is %f\n", other.GetAppearance()->GetMin().x, other.GetAppearance()->GetMin().y, other.GetAppearance()->GetMin().z);
        DebugPrintF("The Other Object's Max x is %f, Max y is %f, Max z is %f\n", other.GetAppearance()->GetMax().x, other.GetAppearance()->GetMax().y, other.GetAppearance()->GetMax().z);
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
    Vector3 closestPoint;
    closestPoint.x = max(_appearance->GetMin().x, min(other.GetPosition().x, _appearance->GetMax().x));
    closestPoint.y = max(_appearance->GetMin().y, min(other.GetPosition().y, _appearance->GetMax().y));
    closestPoint.z = max(_appearance->GetMin().z, min(other.GetPosition().z, _appearance->GetMax().z));

    float distance = sqrt(pow(closestPoint.x - other.GetPosition().x, 2) + 
        pow(closestPoint.y - other.GetPosition().y, 2) + 
        pow(closestPoint.z - other.GetPosition().z, 2));

    if (distance < other.GetRadius())
    {
        DebugPrintF("AABB vs Sphere Collision\n");
        return true;
    }
    else
    {
        return false;
    }
}
