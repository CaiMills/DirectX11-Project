#include "GameObject.h"

GameObject::GameObject()
{
    _texture = nullptr;
}

GameObject::~GameObject()
{

}

void GameObject::LoadGameObjectData(ID3D11Device* _device)
{

}

void GameObject::Update(float deltaTime)
{
    // Calculate world matrix
    XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
    XMMATRIX rotation = XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) * XMMatrixRotationZ(_rotation.z);
    XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

    XMStoreFloat4x4(&_world, XMMatrixIdentity() * scale * rotation * translation);
}

void GameObject::CreateTexture(ID3D11Device* _device, const wchar_t* filePath)
{
    ID3D11ShaderResourceView* _texture;

    CreateDDSTextureFromFile(_device, filePath, nullptr, &_texture);

    SetShaderResource(_texture);
}

void GameObject::Draw(ID3D11DeviceContext* _immediateContext)
{
    //A majority of the draw code need for the game object class (some I couldnt move to this class due to _cbData or something simular)
	if (_meshData.VertexBuffer != nullptr && _meshData.IndexBuffer != nullptr)
	{
		_immediateContext->PSSetShaderResources(0, 1, GetShaderResource());

		_immediateContext->IASetVertexBuffers(0, 1, &GetMeshData()->VertexBuffer, &GetMeshData()->VBStride, &GetMeshData()->VBOffset);
		_immediateContext->IASetIndexBuffer(GetMeshData()->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		_immediateContext->DrawIndexed(GetMeshData()->IndexCount, 0, 0);
	}
}