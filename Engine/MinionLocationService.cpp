#include "Precompiled.h"
#include "MinionLocationService.h"
#include "TransformComponent.h"
#include "BasicBulletComponent.h"
#include "GameObject.h"
#include "XMLReader.h"

MinionLocationService* MinionLocationService::main = nullptr;

Service* MinionLocationService::CreateFunc(const TiXmlNode* node)
{
	MinionLocationService* newService = new MinionLocationService; //do an allocation
	const TiXmlNode* data = node->FirstChild();

	// Delay
	const f32 delay = XMLReader::ReadFloat(data);
	newService->SetDelay(delay);

	return newService;
}

MinionLocationService::MinionLocationService()
	: mCollisionCheckDelay(0.0f)
	, mOriginalDelay(0.0f)
	, mTimer(0.0f)
	, mUpdating(false)
	, mDeleting(false)
{
	if (!main)
	{
		main = this;
	}

	mMinions.get_allocator().allocate(1000);
	mBullets.get_allocator().allocate(1000);
	mDeleteList.get_allocator().allocate(20);
}

MinionLocationService::~MinionLocationService()
{
	mMinions.clear();
}

void MinionLocationService::Update(f32 deltaTime)
{
	mTimer += deltaTime;

	if (!mDeleteList.empty() && !mUpdating)
	{
		DeleteObjects();
	}

	if (mTimer >= mCollisionCheckDelay)
	{
		mTimer += mOriginalDelay;
		if (!mDeleting)
		{
			CheckHitTarget();
		}
	}
}

void MinionLocationService::Initialize()
{
	mOriginalDelay = mCollisionCheckDelay;
}

void MinionLocationService::CheckHitTarget()
{
	mUpdating = true;

	for(GameObject* bullet : mBullets)
	{
		if (bullet)
		{
			Math::Vector3 bulletPos = bullet->GetComponent<TransformComponent>()->GetPosition();
			f32 bulletRange = bullet->GetComponent<BasicBulletComponent>()->GetRange();

			for (GameObject* minion : mMinions)
			{
				if (Math::Distance(minion->GetComponent<TransformComponent>()->GetPosition(), bulletPos) <= bulletRange)
				{
					mDeleteList.push_back(minion);
					mDeleteList.push_back(bullet);
				}
			}
		}
		else
		{
			mDeleteList.push_back(bullet);
		}
	}

	mUpdating = false;
}

void MinionLocationService::RemoveFromList(GameObject& gameObject)
{
	if (!mUpdating)
	{
		if (gameObject.GetComponent<BasicBulletComponent>())
		{
			mBullets.remove(gameObject.GetHandle().Get());
		}
		else if (gameObject.GetComponent<MinionComponent>())
		{
			mMinions.remove(gameObject.GetHandle().Get());
		}
	}
	
	mDeleteList.push_back(gameObject.GetHandle().Get());
}

void MinionLocationService::DeleteObjects()
{
	mDeleting = true;
	u32 size = mDeleteList.size();

	for (int i = 0; i < size; ++i)
	{
		mMinions.remove(mDeleteList.front());
		mBullets.remove(mDeleteList.front());

		if (mDeleteList.front())
		{
			GetWorld().Destroy(mDeleteList.front()->GetHandle());
		}
		mDeleteList.remove(mDeleteList.front());
	}

	mDeleting = false;
}