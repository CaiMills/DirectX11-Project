#pragma once
#include "Structures.h"
#include "OBJLoader.h"
#include "DDSTextureLoader.h"
#include "Globals.h"

struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
};

class Appearance
{
private:
	MeshData _meshData;
	Material _material;
	ID3D11ShaderResourceView* _texture = nullptr;

public:
	Appearance(MeshData meshData);
	~Appearance();

	Material GetMaterial() const { return _material; }
	ID3D11ShaderResourceView** GetTexture() { return &_texture; }

	void SetTexture(ID3D11ShaderResourceView* in) { _texture = in; }
	bool HasTexture() const { return _texture ? true : false; }

	void Draw(ID3D11DeviceContext* pImmediateContext);
};

