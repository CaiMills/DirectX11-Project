#pragma once
#include "PhysicsModel.h"

class RigidBodyModel : public PhysicsModel
{
private:
    Vector3 _torque;

public:
    void AddRelativeForce(Vector3 force, Vector3 point) override;

    //XMStoreFloat3(torque, XMVector3Cross(XMVectorSet(force.x, force.y, force.z, 0), XMVectorSet(point.x, point.y, point.z, 0)))
};