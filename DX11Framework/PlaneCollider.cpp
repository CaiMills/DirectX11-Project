#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

bool PlaneCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
    // Plane vs Sphere Collision
    Vector3 distance = GetPosition() - other.GetPosition();
    GetPosition().Normalize();
    other.GetPosition().Normalize();
    float dotProduct = (GetPosition().x * other.GetPosition().x) + (GetPosition().y * other.GetPosition().y) + (GetPosition().z * other.GetPosition().z);
    float penetrationDepth = other.GetRadius() - dotProduct;

    if (penetrationDepth > 0) 
    {
        return true;
    }

    return false;
}


bool PlaneCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    return false;
}
