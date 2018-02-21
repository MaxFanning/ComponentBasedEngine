#ifndef INCLUDED_ENGINE_SPAWNERCOMPONENT_H
#define INCLUDED_ENGINE_SPAWNERCOMPONENT_H

#include "Component.h"
#include "EngineMath.h"

class SpawnerComponent : public Component
{
public:
	REGISTER_TYPE(SPWN);

	static Component* CreateFunc(const TiXmlNode* node);

	SpawnerComponent();
	~SpawnerComponent() override;

	void Initialize() override;
	virtual void Update(f32 deltaTime) override;
	virtual void Spawn(f32 deltaTime);

	void SetPrefabDirectory(char* direc);
	void SetSpawnDelay(f32 delay) { mSpawnDelay = delay; mOriginalSpawnDelay = delay; }

protected:
	World* mWorld;

	char* mPrefabDirectory;
	std::string mPrefabName;

	f32 mSpawnDelay;
	f32 mOriginalSpawnDelay;
	f32 mTimer;
	s32 mPrefabID;

};

#endif // #ifndef INCLUDED_ENGINE_SPAWNERCOMPONENT_H
