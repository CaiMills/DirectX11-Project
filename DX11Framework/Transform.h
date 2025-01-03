#pragma once
#include "Structures.h"

class Transform
{
private:
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;

public:
	Transform();
	~Transform();

	// Setters and Getters for position/rotation/scale
	void SetPosition(Vector3 position) { _position = position; }
	Vector3 GetPosition() const { return _position; }

	void SetScale(Vector3 scale) { _scale = scale; }
	Vector3 GetScale() const { return _scale; }

	void SetRotation(Vector3 rotation) { _rotation = rotation; }
	Vector3 GetRotation() const { return _rotation; }

	void Move(Vector3 direction);
};

