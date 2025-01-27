#pragma once
#include "Structures.h"
#include "OBJLoader.h"
#include "DDSTextureLoader.h"
#include "Globals.h"

class Appearance
{
private:
	MeshData _meshData;
	ID3D11ShaderResourceView* _texture = nullptr;
	Vector3 _max = Vector3();
	Vector3 _min = Vector3();

public:
	Appearance(MeshData meshData);
	~Appearance();

	ID3D11ShaderResourceView** GetTexture() { return &_texture; }
	void SetTexture(ID3D11ShaderResourceView* in) { _texture = in; }
	bool HasTexture() const { return _texture ? true : false; }

	void SetMinAndMax(ID3D11DeviceContext* _immediateContext, ID3D11Device* _device);
	Vector3 GetMin() const { return _min; }
	Vector3 GetMax() const { return _max; }

	void Draw(ID3D11DeviceContext* pImmediateContext, ID3D11Device* _device);
};

