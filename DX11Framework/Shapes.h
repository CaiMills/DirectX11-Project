#pragma once
#include "Structures.h"

/// <summary>
/// This is just the base class for the other shapes such as cube and pyramid
/// </summary>

class Shapes
{
public:
	Shapes();
	virtual ~Shapes();

	virtual void IndexData();
	virtual void VertexData();

	virtual void Draw();
};

