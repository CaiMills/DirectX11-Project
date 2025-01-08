#include "Appearance.h"

Appearance::Appearance(MeshData meshData) : _meshData(meshData)
{
	_texture = nullptr;
}

Appearance::~Appearance()
{
	_texture = nullptr;
	_meshData.IndexBuffer = nullptr;
	_meshData.VertexBuffer = nullptr;
}

void Appearance::Draw(ID3D11DeviceContext* _immediateContext)
{
	//A majority of the draw code need for the game object class (some I couldnt move to this class due to _cbData or something simular)
	if (_meshData.VertexBuffer != nullptr && _meshData.IndexBuffer != nullptr)
	{
		_immediateContext->PSSetShaderResources(0, 1, &_texture);

		// Set vertex and index buffers
		_immediateContext->IASetVertexBuffers(0, 1, &_meshData.VertexBuffer, &_meshData.VBStride, &_meshData.VBOffset);
		_immediateContext->IASetIndexBuffer(_meshData.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		_immediateContext->DrawIndexed(_meshData.IndexCount, 0, 0);
	}
}
