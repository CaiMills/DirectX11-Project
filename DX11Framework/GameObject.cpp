﻿#include "GameObject.h"

GameObject::GameObject()
{
	_appearance = nullptr;
	_transform = new Transform();
	_physicsModel = new StaticBody(_transform);
}

GameObject::~GameObject()
{
	_appearance = nullptr;
	_transform = nullptr;
	_physicsModel = nullptr;
}

void GameObject::Update(float deltaTime)
{
	if (_transform != nullptr)
	{
		_physicsModel->Update(deltaTime);
		_transform->Update(deltaTime);
	}
}

void GameObject::Draw()
{
	// Write constant buffer data onto GPU
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	if (_appearance->HasTexture())
	{
		_immediateContext->PSSetShaderResources(0, 1, _appearance->GetTexture());
		_cbData.hasTexture = 1.0f;
		_cbData.hasSpecularMap = 1.0f;
	}
	else
	{
		_cbData.hasTexture = 0.0f;
		_cbData.hasSpecularMap = 0.0f;
	}

	_cbData.World = XMMatrixTranspose(_transform->GetWorldMatrix());
	_immediateContext->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
	_immediateContext->Unmap(_constantBuffer, 0);

	_appearance->Draw();
}