#include "RigidBodyModel.h"

RigidBodyModel::RigidBodyModel(Transform* transform) : PhysicsModel(transform)
{
    _transform = transform;
    _mass = 1.0f;

    // Sets the Inertia Tensor to Identity Matrix by default
    XMMATRIX identity = XMMatrixIdentity();
    XMStoreFloat3x3(&_inertiaTensor, identity);

    // If the object has a circle collider, as otherwise the radius will be default 0
    if (GetCollider()->GetRadius() > 0.0f)
    {
        // Circle Matrix
        _inertiaTensor._11 = (2.0f / 5.0f) * GetMass() * (GetCollider()->GetRadius() * GetCollider()->GetRadius());
        _inertiaTensor._12 = 0;
        _inertiaTensor._13 = 0;

        _inertiaTensor._21 = 0;
        _inertiaTensor._22 = (2.0f / 5.0f) * GetMass() * (GetCollider()->GetRadius() * GetCollider()->GetRadius());
        _inertiaTensor._23 = 0;

        _inertiaTensor._31 = 0;
        _inertiaTensor._32 = 0;
        _inertiaTensor._33 = (2.0f / 5.0f) * GetMass() * (GetCollider()->GetRadius() * GetCollider()->GetRadius());
    }
    // If the object has a box collider
    else
    {
        // Box Matrix
        _inertiaTensor._11 = (1.0f / 12.0f) * GetMass() * pow(_transform->GetScale().y / 2, 2) + pow(_transform->GetScale().z / 2, 2);
        _inertiaTensor._12 = 0;
        _inertiaTensor._13 = 0;

        _inertiaTensor._21 = 0;
        _inertiaTensor._22 = (1.0f / 12.0f) * GetMass() * pow(_transform->GetScale().x / 2, 2) + pow(_transform->GetScale().z / 2, 2);
        _inertiaTensor._23 = 0;

        _inertiaTensor._31 = 0;
        _inertiaTensor._32 = 0;
        _inertiaTensor._33 = (1.0f / 12.0f) * GetMass() * pow(_transform->GetScale().x / 2, 2) + pow(_transform->GetScale().y / 2, 2);
    }
}

void RigidBodyModel::AddRelativeForce(Vector3 force, Vector3 point)
{
	AddForce(force);
	
	Vector3 crossProduct;
	crossProduct.x = (force.y * point.z) - (force.z * point.y);
	crossProduct.y = (force.z * point.x) - (force.x * point.z);
	crossProduct.z = (force.x * point.y) - (force.y * point.x);
	
    // Torque = The origin point of the applied force X force
	_torque = crossProduct;
}

void RigidBodyModel::CalculateAngularVelocity(float deltaTime)
{
    if (_mass == 0)
    {
        return
    }
	XMFLOAT3X3 angularAcceleration = XMMatrixInverse(_inertiaTensor) * XMVector3Transform(_torque);
	Vector3 _angularVelocity += angularAcceleration * deltaTime;

    // New Orientation is Calculation (Not sure its meant to placed here)
    Quaternion newOrientation = GetTransform()->GetOrientation() + deltaTime / 2 * _angularVelocity * GetTransform()->GetOrientation();
}

void RigidBodyModel::Update(float deltaTime)
{
	// Linear F=MA 
	PhysicsModel::Update(deltaTime);
}
