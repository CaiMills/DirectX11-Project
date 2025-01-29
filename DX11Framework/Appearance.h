#pragma once
#include "Structures.h"
#include "OBJLoader.h"
#include "DDSTextureLoader.h"
#include "Mesh.h"
#include "Globals.h"

class Appearance
{
private:
	Mesh* _mesh;
	ID3D11ShaderResourceView* _texture = nullptr;

public:
	Appearance(Mesh* mesh);
	~Appearance();

	Mesh* GetMesh() const { return _mesh; }

	ID3D11ShaderResourceView** GetTexture() { return &_texture; }
	void SetTexture(ID3D11ShaderResourceView* texture) { _texture = texture; }
	bool HasTexture() const { return _texture ? true : false; }

	void Draw();
};

