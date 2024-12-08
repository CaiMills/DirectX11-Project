#pragma once
#include "DataFileLoader.h"
#include "OBJLoader.h"
#include "DDSTextureLoader.h"

class GameObject
{
private:
	ID3D11ShaderResourceView* _texture = nullptr;
	MeshData _meshData;
	DirectX::XMFLOAT4X4 _world;

public:
	GameObject();
	~GameObject();

	void SetMeshData(MeshData in) { _meshData = in; }
	void SetShaderResource(ID3D11ShaderResourceView* in) { _texture = in; }
	void SetWorldMatrix(XMFLOAT4X4 in) { _world = in; }

	MeshData* GetMeshData() { return &_meshData; }
	ID3D11ShaderResourceView** GetShaderResource() { return &_texture; }
	XMFLOAT4X4* GetWorldMatrix() { return &_world; }

	void JSONLoad(ID3D11Device* _device);

	void Draw(GameObject gameObject, ID3D11DeviceContext* _immediateContext, ID3D11Buffer* _constantBuffer);
};

