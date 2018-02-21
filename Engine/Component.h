#ifndef COMPONENT_H
#define COMPONENT_H

#include "RTTI.h"
#include "World.h"

class GameObject;

class Component
{
public:
	REGISTER_TYPE(CMPT)

	Component();
	virtual ~Component();

	virtual void Initialize() {}
	virtual void Terminate() {}

	virtual void Update(f32 deltaTime) {}
	virtual void Render() {}
	virtual void Render2D() {}

	GameObject& GetObj() { return *mGameObject; }
	const GameObject& GetObj() const { return *mGameObject; }
	World& GetWorld() { return mGameObject->GetWorld(); }

private:
	friend class GameObject;

	GameObject* mGameObject;
};

#endif // !COMPONENT_H
