#include "RigidBodyModel.h"

RigidBodyModel::RigidBodyModel(Transform* transform) : PhysicsModel(transform)
{
    _mass = GetMass();
    if (_mass == 0.0f)
    {
        _mass = 0.1f;
    }

    _transform = transform;

    // Sets the Inertia Tensor to Identity Matrix by default
    XMMATRIX identity = XMMatrixIdentity();
    XMStoreFloat3x3(&_inertiaTensor, XMMatrixIdentity());
}

void RigidBodyModel::SetInertiaTensor()
{
    Collider* collider = GetCollider();

    // If the object has a circle collider, as otherwise the radius will be default 0
    if (collider->GetRadius() > 0.0f)
    {
        // Circle Matrix
        _inertiaTensor._11 = (2.0f / 5.0f) * _mass * (collider->GetRadius() * collider->GetRadius());
        _inertiaTensor._12 = 0;
        _inertiaTensor._13 = 0;

        _inertiaTensor._21 = 0;
        _inertiaTensor._22 = (2.0f / 5.0f) * _mass * (collider->GetRadius() * collider->GetRadius());
        _inertiaTensor._23 = 0;

        _inertiaTensor._31 = 0;
        _inertiaTensor._32 = 0;
        _inertiaTensor._33 = (2.0f / 5.0f) * _mass * (collider->GetRadius() * collider->GetRadius());
    }
    // If the object has a box collider...
    else
    {
        // Box Matrix
        _inertiaTensor._11 = (1.0f / 12.0f) * _mass * pow(_transform->GetPosition().y / 2, 2) + pow(_transform->GetPosition().z / 2, 2);
        _inertiaTensor._12 = 0;
        _inertiaTensor._13 = 0;

        _inertiaTensor._21 = 0;
        _inertiaTensor._22 = (1.0f / 12.0f) * _mass * pow(_transform->GetPosition().x / 2, 2) + pow(_transform->GetPosition().z / 2, 2);
        _inertiaTensor._23 = 0;

        _inertiaTensor._31 = 0;
        _inertiaTensor._32 = 0;
        _inertiaTensor._33 = (1.0f / 12.0f) * _mass * pow(_transform->GetPosition().x / 2, 2) + pow(_transform->GetPosition().y / 2, 2);
    }
}

void RigidBodyModel::AddRelativeForce(Vector3 force, Vector3 point)
{
	RigidBodyModel::AddForce(force);
    _torque = force ^ point;
}

void RigidBodyModel::CalculateAngularVelocity(float deltaTime)
{
    SetInertiaTensor();

    if (_mass == 0)
    {
        return;
    }
    // Converts Inertia Tensor into a ,atrix which is effected by the Torque variable, which is converted to a Vector
    XMVECTOR torqueVector = XMVectorSet(_torque.x, _torque.y, _torque.z, 0.0f);
    XMMATRIX inertiaMatrix = XMMatrixInverse(nullptr , XMLoadFloat3x3(&_inertiaTensor));
    XMVECTOR angularAccelerationVector = XMVector3Transform(torqueVector, inertiaMatrix);
    XMFLOAT3 angularAcceleration;
    XMStoreFloat3(&angularAcceleration, angularAccelerationVector);

    // Calculates the Angular Velocity
    _angularVelocity += Vector3(angularAcceleration.x, angularAcceleration.y, angularAcceleration.z) * deltaTime;

    // New Orientation is Calculation
    Quaternion orientation = GetTransform()->GetOrientation();
    orientation += orientation * _angularVelocity * 0.5f * deltaTime;

    // Dampens the Angular Velocity overtime
    _angularVelocity *= pow(_angularDamping, deltaTime);

    if (orientation.Magnitude() != 0)
    {
        orientation /= orientation.Magnitude();
    }
    GetTransform()->SetOrientation(orientation);

    // Resets Torque for next calculation
    _torque = Vector3();
}

void RigidBodyModel::Update(float deltaTime)
{
	// Linear F=MA 
	PhysicsModel::Update(deltaTime);
    CalculateAngularVelocity(deltaTime);
}
