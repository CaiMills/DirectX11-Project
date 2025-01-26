#pragma once
#include "Structures.h"
#include "Transform.h"
#include "BoundingBox.h"

/// <summary>
/// This is just the base class for the other shapes such as cube and pyramid
/// </summary>

class Geometry
{
private:
	ID3D11Buffer* _indexBuffer;
	ID3D11Buffer* _vertexBuffer;
	float _noOfIndices;
	bool _isInverted;

public:

	Geometry();
	~Geometry();

	MeshData Cube(ID3D11Device* _device, bool inverted);
	MeshData Pyramid(ID3D11Device* _device);
	MeshData Plane(ID3D11Device* _device);
};

