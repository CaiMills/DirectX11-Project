#pragma once
#include "Structures.h"

class Transform
{
private:
	XMFLOAT3 _position = XMFLOAT3();
	XMFLOAT3 _rotation = XMFLOAT3();
	XMFLOAT3 _scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

public:
	Transform();
	~Transform();

	//XMFLOAT4X4 GetWorld() const { return _world; }


	void SetScale(XMFLOAT3 scale) { _scale = scale; }
	void SetRotation(XMFLOAT3 rotation) { _rotation = rotation; }
	void SetPosition(XMFLOAT3 position) { _position = position; }

	XMFLOAT3 GetPosition() const { return _position; }
	XMFLOAT3 GetScale() const { return _scale; }
	XMFLOAT3 GetRotation() const { return _rotation; }
};

