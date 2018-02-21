#ifndef INCLUDED_ENGINE_TOWERCOMPONENT_H
#define INCLUDED_ENGINE_TOWERCOMPONENT_H

#include "SpawnerComponent.h"
#include "TransformComponent.h"
#include "MinionComponent.h"
#include "EngineMath.h"

struct FindClosestMinionVisitor : public Visitor
{
	// Get Closest Minion
	virtual void operator()(GameObject* go)
	{
		if (go->GetComponent<MinionComponent>() != nullptr)
		{
   			Math::Vector3 minionPosition = go->GetComponent<TransformComponent>()->GetPosition();
			f32 distance = Math::Distance(minionPosition, towerPosition);

			if (distance <= closest && distance <= range/* && go->GetHandle().IsValid()*/)
			{
				closestGO = go;
				//closestGOHandle = go->GetHandle();
				closest = distance;
			}
		}

		if (closestGO != nullptr)
		{
			if (!closestGO->GetHandle().IsValid())
			{
				closestGO = nullptr;
			}
		}
	}

	GameObject* closestGO;
	//GameObjectHandle closestGOHandle;
	f32 closest;
	f32 range;
	Math::Vector3 towerPosition;
};

class TowerComponent : public SpawnerComponent
{
public:
	REGISTER_TYPE(TWER);

	static Component* CreateFunc(const TiXmlNode* node);

	TowerComponent();
	~TowerComponent() override;

	void Initialize() override;
	void Update(f32 deltaTime) override;
	void Spawn(f32 deltaTime) override;

	void SetRange(f32 range) { mRange = range; }

private:
	FindClosestMinionVisitor mVisitor;

	f32 mRange;

};

#endif // #ifndef INCLUDED_ENGINE_TOWERCOMPONENT_H
