#pragma once
#include "Shapes.h"

class Cube : public Shapes
{
protected:
	ID3D11ShaderResourceView* _texture = nullptr;

	ID3D11Buffer* _cubeIndexBuffer;
	ID3D11Buffer* _cubeVertexBuffer;

public:
	Cube();
	~Cube();

	void SetShaderResource(ID3D11ShaderResourceView* in) { _texture = in; }
	ID3D11ShaderResourceView** GetShaderResource() { return &_texture; }

	void IndexData(ID3D11Device* _device, bool inverted);
	void VertexData(ID3D11Device* _device);

	void Draw(ID3D11DeviceContext* _immediateContext, ID3D11Buffer* _constantBuffer);
};

