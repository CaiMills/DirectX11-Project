#pragma once
#include "Structures.h"
#include "Appearance.h"
#include "PhysicsModel.h"
#include "ParticleModel.h"
#include "RigidBodyModel.h"
#include "StaticBody.h"
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
	~GameObject();

	void SetType(string type) { _type = type; }
	void SetAppearance(Appearance* appearance) { _appearance = appearance; }
	void SetPhysicsModel(PhysicsModel* physicsModel) { _physicsModel = physicsModel; }

	string GetType() const { return _type; }
	Appearance* GetAppearance() { return _appearance; }
	Transform* GetTransform() { return _transform; }
	PhysicsModel* GetPhysicsModel() const { return _physicsModel; }

	void Update(float deltaTime);
	void Draw();
};

