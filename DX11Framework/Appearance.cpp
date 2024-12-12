#include "Appearance.h"

Appearance::Appearance()
{
	_texture = nullptr;
}

Appearance::~Appearance()
{
	_texture = nullptr;
	_meshData.IndexBuffer = nullptr;
	_meshData.VertexBuffer = nullptr;
}
