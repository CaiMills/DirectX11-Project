#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

bool PlaneCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
    // Plane vs Sphere Collision
    Vector3 distance = GetPosition() - other.GetPosition();

    // This normal is gotten from the vertex data that the plane mesh was defined with, as all the vertices shared the exact same normal vector, so instead of writing a function to get it, I just manually wrote it
    Vector3 planeNormal = Vector3(0.0f, 0.0f, -1.0f); 

    float dot = (GetPosition().x * other.GetPosition().x) + (GetPosition().y * other.GetPosition().y) + (GetPosition().z * other.GetPosition().z);

    float dotProduct = (other.GetPosition().x - GetPosition().x) * planeNormal.x +
        (other.GetPosition().y - GetPosition().y) * planeNormal.y +
        (other.GetPosition().z - GetPosition().z) * planeNormal.z;


    if (dotProduct <= other.GetRadius())
    {
        out.collisionNormal = distance;
        out.collisionNormal.Normalize();
        out.contactPointCount = 1;
        out.points[0].Position = GetPosition() + (out.collisionNormal * other.GetRadius());
        out.points[0].penetrationDepth = fabs(distance.Magnitude() - other.GetRadius());
        return true;
    }

    return false;
}


bool PlaneCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{

    return false;
}

// Ask for help on the following
// 1) What does the collision manifold calculation actually do
// 2) What is wrong with my quaternion calculations? (its likely angular velocity)
// 3) Maybe help on how plane collision is meant to work