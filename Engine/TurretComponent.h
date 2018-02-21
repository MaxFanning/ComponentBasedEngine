#ifndef INCLUDED_ENGINE_TURRETCOMPONENT_H
#define INCLUDED_ENGINE_TURRETCOMPONENT_H

#include "Component.h"
#include "SpawnerComponent.h"
#include "EngineMath.h"

class TurretComponent : public SpawnerComponent
{
public:
	REGISTER_TYPE(TRRT);

	static Component* CreateFunc(const TiXmlNode* node);

	TurretComponent();
	~TurretComponent() override;

	virtual void Update(f32 deltaTime) override;
	virtual void Spawn();

protected:

};

#endif // #ifndef INCLUDED_ENGINE_TURRETCOMPONENT_H
