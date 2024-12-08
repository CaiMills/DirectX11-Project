#pragma once
#include "Shapes.h"
class Pyramid : public Shapes
{
protected:
	ID3D11Buffer* _pyramidIndexBuffer;
	ID3D11Buffer* _pyramidVertexBuffer;

public:
	Pyramid();
	~Pyramid();

	void IndexData(ID3D11Device* _device);
	void VertexData(ID3D11Device* _device);

	void Draw(ID3D11DeviceContext* _immediateContext, ID3D11Buffer* _constantBuffer);
};

