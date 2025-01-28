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
	//Two constructors, incase the mesh data already exists from an external file
	Mesh() : _mesh({}) {}
	Mesh(MeshData mesh) : _mesh(mesh) { _mesh = mesh; }
	~Mesh();

	void SetMesh(MeshData mesh) { _mesh = mesh; }
	MeshData GetMesh() const { return _mesh; }

	MeshData CreateCube(bool inverted);
	MeshData CreatePyramid();
	MeshData CreatePlane();
};

