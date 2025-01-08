#pragma once
#include "Structures.h"
#include "Appearance.h"
#include "PhysicsModel.h"
#include "Transform.h"

using namespace std;

class GameObject
{
private:
	string _type;
	Appearance* _appearance;
	Transform* _transform = nullptr;
	PhysicsModel* _physicsModel = nullptr;

public:
	GameObject();
	GameObject(string type, Appearance* appearance);
	~GameObject();

	void SetAppearance(Appearance* appearance) { _appearance = appearance; }
	void SetType(string type) { _type = type; }

	string GetType() const { return _type; }
	Appearance* GetAppearance() { return _appearance; }
	Transform* GetTransform() { return _transform; }
	PhysicsModel* GetPhysicsModel() const { return _physicsModel; }

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* _immediateContext);
};

