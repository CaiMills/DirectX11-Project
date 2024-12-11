#pragma once
#include "Structures.h"
#include "OBJLoader.h"
#include "DDSTextureLoader.h"

class GameObject
{
private:
	ID3D11ShaderResourceView* _texture = nullptr;
	MeshData _meshData;

	XMFLOAT3 _position = XMFLOAT3();
	XMFLOAT3 _rotation = XMFLOAT3();
	XMFLOAT3 _scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMFLOAT4X4 _world;

public:
	GameObject();
	~GameObject();

	void SetMeshData(MeshData in) { _meshData = in; }
	void SetShaderResource(ID3D11ShaderResourceView* in) { _texture = in; }
	void SetWorldMatrix(XMFLOAT4X4 in) { _world = in; }

	MeshData* GetMeshData() { return &_meshData; }
	ID3D11ShaderResourceView** GetShaderResource() { return &_texture; }
	//bool HasTexture() const { return _texture ? true : false; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void SetScale(XMFLOAT3 scale) { _scale = scale; }
	void SetRotation(XMFLOAT3 rotation) { _rotation = rotation; }
	void SetPosition(XMFLOAT3 position) { _position = position; }

	XMFLOAT3 GetPosition() const { return _position; }
	XMFLOAT3 GetScale() const { return _scale; }
	XMFLOAT3 GetRotation() const { return _rotation; }

	void LoadGameObjectData(ID3D11Device* _device);

	void Update(float deltaTime);

	void Draw(ID3D11DeviceContext* _immediateContext);
};

