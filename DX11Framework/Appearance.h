#pragma once
#include "Structures.h"
#include "OBJLoader.h"
#include "DDSTextureLoader.h"

class Appearance
{
private:
	MeshData _meshData;
	ID3D11ShaderResourceView* _texture = nullptr;

public:
	Appearance();
	~Appearance();

	void SetMeshData(MeshData in) { _meshData = in; }
	void SetTexture(ID3D11ShaderResourceView* in) { _texture = in; }

	MeshData* GetMeshData() { return &_meshData; }
	ID3D11ShaderResourceView** GetTexture() { return &_texture; }

	bool HasTexture() const { return _texture ? true : false; }
};

