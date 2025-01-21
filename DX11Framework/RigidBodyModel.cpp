#include "RigidBodyModel.h"

void RigidBodyModel::AddRelativeForce(Vector3 force, Vector3 point)
{
	AddForce(force);

	Vector3 crossProduct;
	crossProduct.x = (force.y * point.z) - (force.z * point.y);
	crossProduct.y = (force.z * point.x) - (force.x * point.z);
	crossProduct.z = (force.x * point.y) - (force.y * point.x);

	//XMStoreFloat3(&cross, XMVector3Cross(XMVectorSet(force.x, force.y, force.z, 0), XMVectorSet(point.x, point.y, point.z, 0)));

	_torque = crossProduct;
}
