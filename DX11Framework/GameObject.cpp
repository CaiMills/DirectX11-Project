#include "GameObject.h"

GameObject::GameObject()
{
	_transform = new Transform();
	_physicsModel = new PhysicsModel(_transform);
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

void GameObject::Draw(ID3D11DeviceContext* _immediateContext)
{
	_appearance->Draw(_immediateContext);
}