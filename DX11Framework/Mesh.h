#pragma once
#include "Structures.h"
#include "Transform.h"
#include "Globals.h"

/// <summary>
/// This is just the base class for the other shapes such as cube and pyramid
/// </summary>

class Mesh
{
private:
	MeshData _mesh = {};
	ID3D11Buffer* _vertexBuffer = nullptr;
	ID3D11Buffer* _indexBuffer = nullptr;
	//UINT _indexCount = 0;
	bool _isInverted = false;

public:
	Mesh();
	~Mesh();

	MeshData Cube(bool inverted);
	MeshData Pyramid();
	MeshData Plane();
};

