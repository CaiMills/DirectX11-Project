#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

bool PlaneCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
    // Plane vs Sphere Collision
    Vector3 distance = GetPosition() - other.GetPosition();
    distance.Normalize();

    // This normal is gotten from the vertex data that the plane mesh was defined with, as all the vertices shared the exact same normal vector, so instead of writing a function to get it, I just manually wrote it
    Vector3 planeNormal = Vector3(0.0f, 0.0f, -1.0f); 

    float dot = (distance.x * planeNormal.x) + (distance.y * planeNormal.y) + (distance.z * planeNormal.z);
    float angle = acos(dot);

    Vector3 answer = distance * angle;

    if (answer.Magnitude() <= other.GetRadius())
    {
        return true;
    }

    return false;
}


bool PlaneCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    // Plane vs AABB Collision
    Vector3 distance = GetPosition() - other.GetPosition();

    Vector3 boxAExtents = GetAppearance()->GetMesh()->GetExtents();
    Vector3 boxBExtents = other.GetAppearance()->GetMesh()->GetExtents();
    Vector3 halfExtents = (boxAExtents / 2) + (boxBExtents / 2);

    if (distance.x <= halfExtents.x && distance.y <= halfExtents.y && distance.z <= halfExtents.z)
    {
        //Need to add Manifold Stuff
        return true;
    }

    return false;
}