#include "Appearance.h"

Appearance::Appearance(Mesh* mesh) : _mesh(mesh)
{
	_texture = nullptr;
}

Appearance::~Appearance()
{
	_texture = nullptr;
	_mesh->GetMeshData()->IndexBuffer = nullptr;
	_mesh->GetMeshData()->VertexBuffer = nullptr;
}

void Appearance::Draw()
{
	if (_mesh->GetMeshData()->VertexBuffer != nullptr && _mesh->GetMeshData()->IndexBuffer != nullptr)
	{
		// Set texture
		_immediateContext->PSSetShaderResources(0, 1, &_texture);

		// Set vertex and index buffers
		_immediateContext->IASetVertexBuffers(0, 1, &_mesh->GetMeshData()->VertexBuffer, &_mesh->GetMeshData()->VBStride, &_mesh->GetMeshData()->VBOffset);
		_immediateContext->IASetIndexBuffer(_mesh->GetMeshData()->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		_immediateContext->DrawIndexed(_mesh->GetMeshData()->IndexCount, 0, 0);
	}
}
