#ifndef MINION_LOCATION_SERVICE_HEADER_H
#define MINION_LOCATION_SERVICE_HEADER_H

#include "RTTI.h"
#include "Service.h"
#include "World.h"

class MinionLocationService : public Service
{
public:
	static MinionLocationService* main;
	REGISTER_TYPE(MNLC)

	static Service* CreateFunc(const TiXmlNode* node);

	MinionLocationService();
	virtual ~MinionLocationService();

	void Update(f32 deltaTime) override;
	void Initialize() override;

	void AddMinion(GameObject* minion) { mMinions.push_back(minion); }
	void AddBullet(GameObject* bullet) { mBullets.push_back(bullet); }
	void CheckHitTarget();
	void RemoveFromList(GameObject& gameObject);

	void SetDelay(f32 delay) { mCollisionCheckDelay = delay; }

private:
	void DeleteObjects();

	std::list<GameObject*> mMinions;
	std::list<GameObject*> mBullets;
	std::list<GameObject*> mDeleteList;

	f32 mCollisionCheckDelay;
	f32 mOriginalDelay;
	f32 mTimer;

	bool mUpdating;
	bool mDeleting;

};

#endif
