#pragma once
#include "Structures.h"
#include "Transform.h"

/// <summary>
/// This is just the base class for the other shapes such as cube and pyramid
/// </summary>

class Shapes
{
private:
	XMFLOAT4X4 _world;

public:
	Transform* _transform = nullptr;

	Shapes();
	virtual ~Shapes();

	virtual void IndexData();
	virtual void VertexData();

	Transform* GetTransform() { return _transform; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	virtual void Draw();
};

