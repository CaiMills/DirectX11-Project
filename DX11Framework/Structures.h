#pragma once
#include <windows.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <iostream>
#include "JSON\json.hpp"
#include "vector"
#include <string>
#include <locale>
#include <codecvt> // Needed for older compilers (deprecated in C++17, but useful here)
#include "Vector3.h"
#include <cstring>

using namespace DirectX;

struct ConstantBuffer
{
	//16bit
	XMMATRIX Projection;
	XMMATRIX View;
	XMMATRIX World;

	//16bit
	XMFLOAT4 diffuseLight;
	XMFLOAT4 diffuseMaterial;
	XMFLOAT4 ambientLight;
	XMFLOAT4 ambientMaterial;

	//16bit
	XMFLOAT4 specularLight;
	XMFLOAT4 specularMaterial;
	XMFLOAT3 cameraPosition;
	float specPower;

	//3bit
	XMFLOAT3 lightDir;

	//Texture
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

//Mask namespace for shorthand
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
	std::string specularTexture;
	std::string colorTexture;
	XMFLOAT3 startScale;
	XMFLOAT3 startRot;
	XMFLOAT3 startPos;
};

