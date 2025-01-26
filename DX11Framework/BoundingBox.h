#pragma once
#include "Structures.h"

class BoundingBox
{
private:
	XMFLOAT3 _min;
	XMFLOAT3 _max;

public:
	BoundingBox();
	~BoundingBox();

	void CalculateBox(std::vector<SimpleVertex> vertexData);
};

