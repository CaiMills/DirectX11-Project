//#pragma once
//#include "Structures.h"
//#include "Transform.h"
//
///// <summary>
///// This is just the base class for the other shapes such as cube and pyramid
///// </summary>
//
//class Geometry
//{
//private:
//	ID3D11ShaderResourceView* _texture;
//
//	ID3D11Buffer* _indexBuffer;
//	ID3D11Buffer* _vertexBuffer;
//	float _noOfIndices;
//	bool _isInverted;
//
//public:
//
//	Geometry();
//	~Geometry();
//
//	void CubeData(ID3D11Device* _device, bool inverted);
//	void PyramidData(ID3D11Device* _device);
//	void PlaneData(ID3D11Device* _device);
//
//	virtual void Draw(ID3D11DeviceContext* _immediateContext);
//};
//
