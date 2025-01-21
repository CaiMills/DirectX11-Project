#pragma once
#include "PhysicsModel.h"

class RigidBodyModel : public PhysicsModel
{
private:
    Vector3 _torque;
    XMFLOAT3X3 _inertiaTensor;
    float _angularDamping = 0.99f;

public:
    RigidBodyModel(Transform* transform) : PhysicsModel(transform) 
    {
        //Sets the Inertia Tensor to Identity Matrix by default
        XMMATRIX identity = XMMatrixIdentity();
        XMStoreFloat3x3(&_inertiaTensor, identity);
    };

    void AddRelativeForce(Vector3 force, Vector3 point) override;

    //XMStoreFloat3(torque, XMVector3Cross(XMVectorSet(force.x, force.y, force.z, 0), XMVectorSet(point.x, point.y, point.z, 0)))
};