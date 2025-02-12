#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

bool PlaneCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
    // Plane vs Sphere Collision
    
    // This counts as a collision if the object moves down, no matter how far away it is from the plane
    //Vector3 distance = GetPosition() - other.GetPosition();
    //GetPosition().Normalize();
    //other.GetPosition().Normalize();
    //float dotProduct = (GetPosition().x * other.GetPosition().x) + (GetPosition().y * other.GetPosition().y) + (GetPosition().z * other.GetPosition().z);
    //float penetrationDepth = other.GetRadius() - dotProduct;

    Vector3 distance = GetPosition() - other.GetPosition();
    Vector3 closestPoint;
    closestPoint.x = other.GetPosition().x - distance.Magnitude();
    closestPoint.y = other.GetPosition().y - distance.Magnitude();
    closestPoint.z = other.GetPosition().z - distance.Magnitude();

    if (closestPoint.Magnitude() > 0)
    {
        return true;
    }

    return false;
}


bool PlaneCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    return false;
}
