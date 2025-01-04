#pragma once
#include "Structures.h"
#include "Appearance.h"
#include "PhysicsModel.h"
#include "Transform.h"

class GameObject
{
private:
	Appearance* _appearance = nullptr;
	Transform* _transform = nullptr;
	PhysicsModel* _physicsModel = nullptr;

public:
	GameObject();
	~GameObject();

	Appearance* GetAppearance() { return _appearance; }
	Transform* GetTransform() { return _transform; }
	PhysicsModel* GetPhysicsModel() const { return _physicsModel; }

	void SetAppearance(Appearance* in) { _appearance = in; }

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* _immediateContext);
};

