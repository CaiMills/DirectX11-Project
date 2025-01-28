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
	Vector3 _max = Vector3();
	Vector3 _min = Vector3();
	bool _isInverted = false;

public:
	//Two constructors, in case the mesh data already exists from an external file
	Mesh() : _mesh({}) {}
	Mesh(MeshData mesh) : _mesh(mesh) { _mesh = mesh; }
	~Mesh();

	void SetMesh(MeshData mesh) { _mesh = mesh; }
	MeshData GetMeshData() const { return _mesh; }

	MeshData CreateCube(bool inverted);
	MeshData CreatePyramid();
	MeshData CreatePlane();

	void SetMinAndMax();
	Vector3 GetMin() const { return _min; }
	Vector3 GetMax() const { return _max; }
};

