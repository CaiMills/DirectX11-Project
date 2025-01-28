#include "Appearance.h"

Appearance::Appearance(MeshData mesh) : _mesh(mesh)
{
	_texture = nullptr;
}

Appearance::~Appearance()
{
	_texture = nullptr;
	_mesh.IndexBuffer = nullptr;
	_mesh.VertexBuffer = nullptr;
}

void Appearance::Draw()
{
	//DebugPrintF("Max x is %f, Max y is %f, Max z is %f\n", _max.x, _max.y, _max.z);
	//DebugPrintF("Min x is %f, Min y is %f, Min z is %f\n", _min.x, _min.y, _min.z);
	if (_mesh.VertexBuffer != nullptr && _mesh.IndexBuffer != nullptr)
	{
		//Set texture
		_immediateContext->PSSetShaderResources(0, 1, &_texture);

		//Set vertex and index buffers
		_immediateContext->IASetVertexBuffers(0, 1, &_mesh.VertexBuffer, &_mesh.VBStride, &_mesh.VBOffset);
		_immediateContext->IASetIndexBuffer(_mesh.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		_immediateContext->DrawIndexed(_mesh.IndexCount, 0, 0);
	}
}
