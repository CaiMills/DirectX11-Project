#pragma once
#include "PhysicsModel.h"

class RigidBodyModel : public PhysicsModel
{
private:
    XMFLOAT3X3 _inertiaTensor;
    Vector3 _torque = Vector3();
    float _angularDamping = 0.99f; // 1 means it will never stop rotating, 0 means it will stop instantly
    Vector3 _angularVelocity = Vector3();
    Transform* _transform = nullptr;

public:
    RigidBodyModel(Transform* transform);

    void SetMass(float mass) override { _mass = mass; }
    Transform* GetTransform() { return _transform; }

    void AddRelativeForce(Vector3 force, Vector3 point) override;
    void CalculateAngularVelocity(float deltaTime);

    virtual void Update(float deltaTime) override;
};