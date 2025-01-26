#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	_min = XMFLOAT3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()); //Sets the variable to lowest possible value
	_max = XMFLOAT3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::CalculateBox(SimpleVertex* vertexData)
{
	for (XMFLOAT3 vertices : vertexData->Position)
	{
		//if the vertex's position is more than the current max value, then make that the new max value
		if (vertices.x > _max.x || vertices.y > _max.y || vertices.z > _max.z)
		{
			_max = vertices;
		}
		//if the vertex's position is less than the current min value, then make that the new min value
		else if (vertices.x > _min.x || vertices.y > _min.y || vertices.z > _min.z)
		{
			_min = vertices;
		}
	}
}
