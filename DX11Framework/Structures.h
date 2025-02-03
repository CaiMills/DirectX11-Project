#pragma once
#include <windows.h>
#include <codecvt> // Needed for older compilers (deprecated in C++17, but useful here)
#include <atlstr.h> // to use CString.
#include <cstring>
#include <d3d11.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <iostream>
#include "JSON\json.hpp"
#include <string>
#include <locale>
#include "Vector3.h"
#include <cstring>
#include "Debug.h"
#include <vector>

using namespace DirectX;

struct ConstantBuffer
{
	// 16 Bit
	XMMATRIX Projection;
	XMMATRIX View;
	XMMATRIX World;

	// 16 Bit
	XMFLOAT4 diffuseLight;
	XMFLOAT4 diffuseMaterial;
	XMFLOAT4 ambientLight;
	XMFLOAT4 ambientMaterial;

	// 16 Bit
	XMFLOAT4 specularLight;
	XMFLOAT4 specularMaterial;
	XMFLOAT3 cameraPosition;
	float specPower;

	// 3 Bit
	XMFLOAT3 lightDir;

	// Texture
	int hasTexture;
	int hasSpecularMap;
};

struct SimpleVertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	};
};

struct MeshData
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
};

struct ContactPoint
{
	Vector3 Position;
	float penetrationDepth;
};

struct CollisionManifold
{
	int contactPointCount = 0;
	ContactPoint points[4];
	Vector3 collisionNormal;
};

// Mask namespace for shorthand
using json = nlohmann::json;

struct lightingData
{
	std::string lightFile;
	XMFLOAT4 light;
	XMFLOAT4 material;
};

struct gameObjectData
{
	std::string objFilePath;
	std::string type;
	std::string specularTexture;
	std::string colorTexture;
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 position;
};