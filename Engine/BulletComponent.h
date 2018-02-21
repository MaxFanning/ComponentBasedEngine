#ifndef INCLUDED_ENGINE_BULLETCOMPONENT_H
#define INCLUDED_ENGINE_BULLETCOMPONENT_H

#include "Component.h"
#include "TransformComponent.h"
#include "EngineMath.h"

class BulletComponent : public Component
{
public:
	REGISTER_TYPE(BLLT);

	static Component* CreateFunc(const TiXmlNode* node);

	BulletComponent();
	~BulletComponent() override;

	void Initialize() override;
	void Update(f32 deltaTime) override;

	void SetTarget(GameObject* targ);
	void SetSpeed(f32 spd);

private:
	TransformComponent* mTransform;
	GameObject* mTarget;
	TransformComponent* mTargetTransform;
	Math::Vector3 mPosition;
	f32 mSpeed;
	f32 mRange;

};

#endif // #ifndef INCLUDED_ENGINE_BULLETCOMPONENT_H
