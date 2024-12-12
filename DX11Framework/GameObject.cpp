#include "GameObject.h"

GameObject::GameObject()
{
	_appearance = nullptr;
}

GameObject::~GameObject()
{
	_appearance = nullptr;
	_transform = nullptr;
}

void GameObject::Update(float deltaTime)
{
	static float rotationAngle = 0.0f; // Initialize once
	rotationAngle += (deltaTime * 2); // Adjust rotation speed as needed

    // Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(GetTransform()->GetScale().x, GetTransform()->GetScale().y, GetTransform()->GetScale().z);
	XMMATRIX rotation = XMMatrixRotationX(rotationAngle) * XMMatrixRotationY(rotationAngle) * XMMatrixRotationZ(GetTransform()->GetRotation().z);
	XMMATRIX translation = XMMatrixTranslation(GetTransform()->GetPosition().x, GetTransform()->GetPosition().y, GetTransform()->GetPosition().z);

	XMStoreFloat4x4(&_world, XMMatrixIdentity() * scale * rotation * translation);
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