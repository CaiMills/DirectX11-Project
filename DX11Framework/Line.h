#pragma once
#include "Geometry.h"

/// <summary>
/// NOTE: Line no longer works as Color has been removed for Normal
/// </summary>

class Line : public Geometry
{
protected:
	ID3D11Buffer* _lineVertexBuffer;

public:
	Line();
	~Line();

	void VertexData(ID3D11Device* _device);

	void Draw(ID3D11DeviceContext* _immediateContext, ID3D11Buffer* _constantBuffer);
};

