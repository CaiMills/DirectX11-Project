#pragma once
#include "PhysicsModel.h"

class RigidBodyModel : public PhysicsModel
{
private:
    Vector3 _torque = Vector3();
    XMFLOAT3X3 _inertiaTensor;
    float _angularDamping = 0.99f;
    Vector3 _angularVelocity = Vector3();

public:
    RigidBodyModel(Transform* transform) : PhysicsModel(transform) 
    {
        //Sets the Inertia Tensor to Identity Matrix by default
        XMMATRIX identity = XMMatrixIdentity();
        XMStoreFloat3x3(&_inertiaTensor, identity);

        ////Circle
        //_inertiaTensor._11 = (2.0f / 5.0f) * _mass * (GetCollider()->GetRadius() * GetCollider()->GetRadius());
        //_inertiaTensor._12 = 0;
        //_inertiaTensor._13 = 0;

        //_inertiaTensor._21 = 0;
        //_inertiaTensor._22 = (2.0f / 5.0f) * _mass * (GetCollider()->GetRadius() * GetCollider()->GetRadius());
        //_inertiaTensor._23 = 0;

        //_inertiaTensor._31 = 0;
        //_inertiaTensor._32 = 0;
        //_inertiaTensor._33 = (2.0f / 5.0f) * _mass * (GetCollider()->GetRadius() * GetCollider()->GetRadius());

        ////Box 
        //_inertiaTensor._11 = (1.0f / 12.0f) * _mass * (((_transform->GetScale().y/2) * (_transform->GetScale().y/2)) + ((_transform->GetScale().z/2) * (_transform->GetScale().z/2)));
        //_inertiaTensor._12 = 0;
        //_inertiaTensor._13 = 0;

        //_inertiaTensor._21 = 0;
        //_inertiaTensor._22 = (1.0f / 12.0f) * _mass * (((_transform->GetScale().x / 2) * (_transform->GetScale().x / 2)) + ((_transform->GetScale().z / 2) * (_transform->GetScale().z / 2)));
        //_inertiaTensor._23 = 0;

        //_inertiaTensor._31 = 0;
        //_inertiaTensor._32 = 0;
        //_inertiaTensor._33 = (1.0f / 12.0f) * _mass * (((_transform->GetScale().x / 2)) * ((_transform->GetScale().x / 2)) + ((_transform->GetScale().y / 2) * (_transform->GetScale().y / 2)));
    };

    void AddRelativeForce(Vector3 force, Vector3 point) override;
    //void CalculateAngularVelocity();
};