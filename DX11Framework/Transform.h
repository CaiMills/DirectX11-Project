#pragma once
#include "Structures.h"
#include "Quaternion.h"

class Transform
{
private:
	Vector3 _position;
	Quaternion _orientation;
	Vector3 _scale;
	XMFLOAT4X4 _world;

public:
	Transform();
	~Transform();

	// Setters and Getters for position/rotation/scale
	void SetPosition(Vector3 position) { _position = position; }
	Vector3 GetPosition() const { return _position; }

	void SetScale(Vector3 scale) { _scale = scale; }
	Vector3 GetScale() const { return _scale; }

	void SetRotation(Vector3 rotation) { _orientation = MakeQFromEulerAngles(rotation.x, rotation.y, rotation.z); }
	Vector3 GetRotation() const { return MakeEulerAnglesFromQ(_orientation); }
	Quaternion GetOrientation(Quaternion orientation) { _orientation = orientation; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void Update(float deltaTime);
	void Move(Vector3 direction);
};

