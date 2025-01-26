#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	//Sets the variables to lowest possible value
	_min = XMFLOAT3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	_max = XMFLOAT3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::CalculateBox(std::vector<SimpleVertex> vertexData)
{
	//for (auto& vertex : vertexData)
	//{
	//	XMFLOAT3 pos = vertex.Position;

	//	//if the vertex's position is more than the current max value, then make that the new max value
	//	if (pos.x > _max.x || pos.y > _max.y || pos.z > _max.z)
	//	{
	//		_max = pos;
	//	}
	//	//if the vertex's position is less than the current min value, then make that the new min value
	//	else if (pos.x > _min.x || pos.y > _min.y || pos.z > _min.z)
	//	{
	//		_min = pos;
	//	}
	//}
}
