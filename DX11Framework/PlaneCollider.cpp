#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

bool PlaneCollider::CollidesWith(SphereCollider& other, CollisionManifold& out)
{
    // Plane vs Sphere Collision
    Vector3 distance = GetPosition() - other.GetPosition();
    float distanceToSphereCenter = distance.Magnitude() - this->GetPosition().Magnitude();
    float penetrationDepth = other.GetRadius() - distanceToSphereCenter;

    if (penetrationDepth > 0) {
        out.collisionNormal = distance;
        out.collisionNormal.Normalize();
        out.contactPointCount = 1;
        out.points[0].Position = GetPosition() + (out.collisionNormal * GetRadius());
        out.points[0].penetrationDepth = fabs(distanceToSphereCenter - other.GetRadius());
        return true;
    }

    return false;
}


bool PlaneCollider::CollidesWith(BoxCollider& other, CollisionManifold& out)
{
    return false;
}
