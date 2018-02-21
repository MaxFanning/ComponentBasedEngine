#ifndef INCLUDED_ENGINE_BASIC_BULLETCOMPONENT_H
#define INCLUDED_ENGINE_BASIC_BULLETCOMPONENT_H

#include "Component.h"
#include "TransformComponent.h"
#include "EngineMath.h"

class BasicBulletComponent : public Component
{
public:
	REGISTER_TYPE(BBLT);

	static Component* CreateFunc(const TiXmlNode* node);

	BasicBulletComponent();
	~BasicBulletComponent() override;

	void Initialize() override;
	void Update(f32 deltaTime) override;

	void SetSpeed(f32 spd);
	void SetDirection(Math::Vector3 forward);
	void SetLifeSpan(f32 lifespan);

	f32 GetRange() { return mRange; }

private:
	TransformComponent* mTransform;
	
	Math::Vector3 mForwardVector;
	Math::Vector3 mPosition;
	f32 mSpeed;
	f32 mRange;
	f32 mLifeSpan;
	f32 mTimer;

};

#endif // #ifndef INCLUDED_ENGINE_BASIC_BULLETCOMPONENT_H
