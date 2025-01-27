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
	XMFLOAT3 _max = XMFLOAT3();
	XMFLOAT3 _min = XMFLOAT3();

public:
	Appearance(MeshData meshData);
	~Appearance();

	ID3D11ShaderResourceView** GetTexture() { return &_texture; }
	void SetTexture(ID3D11ShaderResourceView* in) { _texture = in; }
	bool HasTexture() const { return _texture ? true : false; }

	void SetMaxAndMin(ID3D11DeviceContext* _immediateContext, ID3D11Device* _device);
	XMFLOAT3 GetMax() { return _max; }
	XMFLOAT3 GetMin() { return _min; }

	void Draw(ID3D11DeviceContext* pImmediateContext, ID3D11Device* _device);
};

