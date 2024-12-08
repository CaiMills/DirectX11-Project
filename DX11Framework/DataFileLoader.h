#pragma once
#include <iostream>
#include "JSON\json.hpp"
#include "vector"
#include "Structures.h"

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
	XMFLOAT3 startPos;
	float startScale;
};