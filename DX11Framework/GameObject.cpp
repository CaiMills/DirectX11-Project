#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::Update(float deltaTime)
{
    // Calculate world matrix
    XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
    XMMATRIX rotation = XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) * XMMatrixRotationZ(_rotation.z);
    XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

    XMStoreFloat4x4(&_world, scale * rotation * translation);
}

void GameObject::Draw(ID3D11DeviceContext* _immediateContext)
{
    //A majority of the draw code need for the game object class (some I couldnt move to this class due to _cbData or something simular)
	if (_appearance->GetMeshData()->VertexBuffer != nullptr && _appearance->GetMeshData()->IndexBuffer != nullptr)
	{
		_immediateContext->PSSetShaderResources(0, 1, _appearance->GetTexture());

		_immediateContext->IASetVertexBuffers(0, 1, &_appearance->GetMeshData()->VertexBuffer, &_appearance->GetMeshData()->VBStride, &_appearance->GetMeshData()->VBOffset);
		_immediateContext->IASetIndexBuffer(_appearance->GetMeshData()->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		_immediateContext->DrawIndexed(_appearance->GetMeshData()->IndexCount, 0, 0);
	}
}