#ifndef INCLUDED_ENGINE_MINIONCOMPONENT_H
#define INCLUDED_ENGINE_MINIONCOMPONENT_H

#include "Component.h"
#include "TransformComponent.h"
#include "EngineMath.h"

class Terrain;

class MinionComponent : public Component
{
public:
	REGISTER_TYPE(MNON);

	static Component* CreateFunc(const TiXmlNode* node);

	MinionComponent();
	~MinionComponent() override;

	void Initialize() override;
	void Update(f32 deltaTime) override;
	void SetDestination(Math::Vector3 dest);
	void SetSpeed(f32 spd);
	void SetLifeSpan(f32 span);

private:
	TransformComponent* mTransform;
	Terrain* mTerrain;

	Math::Vector3 mDestination;
	Math::Vector3 mPosition;
	float mOriginalY;
	f32 mSpeed;
	f32 mLifeSpan;
	f32 mAliveTimer;

};

#endif // #ifndef INCLUDED_ENGINE_MINIONCOMPONENT_H
