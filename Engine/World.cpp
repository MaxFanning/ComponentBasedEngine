#include "Precompiled.h"

#include "World.h"
#include "Utils.h"
#include "XMLReader.h"

World::World()
	: mUpdating(false)
{
}

World::~World()
{
}

void World::Intialize(u32 capacity)
{
	mGameObjectAllocator = std::make_unique<GameObjectAllocator>(capacity);
	mGameObjectHandlePool = std::make_unique<GameObjectHandlePool>(capacity);
	mServiceAllocator = std::make_unique<ServiceAllocator>(capacity);

	mGameObjectFactory = std::make_unique<GameObjectFactory>(*mGameObjectAllocator);
	mGameObjectFactory->Register("TransformComponent", TransformComponent::CreateFunc);
	mGameObjectFactory->Register("ColliderComponent", ColliderComponent::CreateFunc);
	mGameObjectFactory->Register("CameraComponent", CameraComponent::CreateFunc);
	mGameObjectFactory->Register("FPSCameraControllerComponent", FPSCameraControllerComponent::CreateFunc);
	mGameObjectFactory->Register("MinionComponent", MinionComponent::CreateFunc);
	mGameObjectFactory->Register("SpawnerComponent", SpawnerComponent::CreateFunc);
	mGameObjectFactory->Register("TowerComponent", TowerComponent::CreateFunc);
	mGameObjectFactory->Register("BulletComponent", BulletComponent::CreateFunc);
	mGameObjectFactory->Register("ModelComponent", ModelComponent::CreateFunc);
	mGameObjectFactory->Register("BasicBulletComponent", BasicBulletComponent::CreateFunc);
	mGameObjectFactory->Register("TurretComponent", TurretComponent::CreateFunc);

	mServiceFactory = std::make_unique<ServiceFactory>(*mServiceAllocator);
	mServiceFactory->Register("MinionLocationService", MinionLocationService::CreateFunc);
}

void World::Terminate()
{
	ASSERT(!mUpdating, "[World] Cannot terminate while updating!");

	for (auto gameObject : mUpdateList)
	{
		mDestroyList.push_back(gameObject->GetHandle());
	}
	mUpdateList.clear();

	PurgeDestroyList();

	mGameObjectFactory.reset();
	mGameObjectHandlePool.reset();
	mGameObjectAllocator.reset();
	mServiceFactory.reset();
}

GameObjectHandle World::Create(const char* templateFileName, const char* name)
{
	GameObject* gameObject = mGameObjectFactory->Create(templateFileName);
	ASSERT(gameObject != nullptr, "[World] failed to create game object from template %s.", templateFileName)

	// Register with the handle pool
		GameObjectHandle handle = mGameObjectHandlePool->Register(gameObject);
	ASSERT(handle.IsValid(), "[World] Failed to create handle for game object!");

	// Initialize gameObject
	gameObject->mWorld = this;
	gameObject->mName = name;
	gameObject->mHandle = handle;
	gameObject->Initialize();

	// Add gameObject to Update List
	mUpdateList.push_back(gameObject);

	return handle;
}

void World::LoadLevel(const char* level)
{
	TiXmlDocument document(level);

	if (!document.LoadFile())
	{
		LOG("[World] Failed to open template file '%s'.", level);
		return;
	}
	const TiXmlNode* nodeType = document.FirstChild();

	if (Utils::AreEqual(nodeType->Value(), "Level"))
	{
		nodeType = nodeType->FirstChild();

		while (nodeType)
		{
			if (Utils::AreEqual(nodeType->Value(), "GameObjects"))
			{
				const TiXmlNode* gameObjectNode = nodeType->FirstChild();

				while (gameObjectNode)
				{
					GameObject* gameObject = mGameObjectAllocator->New();
					const char* gameObjectName = gameObjectNode->Value();

					const TiXmlNode* directory = gameObjectNode->FirstChild();
					const char* dir = XMLReader::ReadString(directory);
					gameObject = mGameObjectFactory->Create(dir);

					// Initialize gameObject
					gameObject->mWorld = this;
					gameObject->mName = gameObjectName;
					gameObject->Initialize();

					// Add gameObject to Update List
					mUpdateList.push_back(gameObject);

					gameObjectNode = gameObjectNode->NextSibling();
				}
			}
			else if (Utils::AreEqual(nodeType->Value(), "Services"))
			{
				const TiXmlNode* serviceNode = nodeType->FirstChild();

				while (serviceNode)
				{
					Service* service = new Service();
					const char* serviceName = serviceNode->Value();

					const TiXmlNode* directory = serviceNode->FirstChild();
					const char* dir = XMLReader::ReadString(directory);
					service = mServiceFactory->Create(dir);

					service->mWorld = this;
					service->mName = serviceName;
					service->Initialize();

					mServices.push_back(service);

					serviceNode = serviceNode->NextSibling();
				}
			}
			nodeType = nodeType->NextSibling();
		}
	}

}

GameObjectHandle World::Find(const char* name)
{
	for (auto gameObject : mUpdateList)
	{
		if (Utils::AreEqual(gameObject->GetName(), name))
		{
			return gameObject->GetHandle();
		}
	}

	return GameObjectHandle();
}

void World::Destroy(GameObjectHandle handle)
{
	if (!handle.IsValid())
	{
		return;
	}

	if (!mUpdating)
	{
		DestroyInternal(handle);
	}
	else
	{
		mDestroyList.push_back(handle);
	}
}

void World::AddService(Service* service)
{
	service->mWorld = this;
	mServices.push_back(service);
}

void World::Visit(Visitor& v)
{
	for (auto gameObject : mUpdateList)
	{
		v(gameObject);
	}
}

void World::Update(f32 deltaTime)
{
	ASSERT(!mUpdating, "[World] Alreadying updating the world!");

	mUpdating = true;

	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObject = mUpdateList[i];
		gameObject->Update(deltaTime);
	}
	for (size_t i = 0; i < mServices.size(); i++)
	{
		Service* service = mServices[i];
		service->Update(deltaTime);
	}

	mUpdating = false;

	PurgeDestroyList();
}

void World::Render()
{
	for (auto gameObject : mUpdateList)
	{
		gameObject->Render();
	}
	for (auto service : mServices)
	{
		service->Render();
	}
}

void World::Render2D()
{
	for (auto gameObject : mUpdateList)
	{
		gameObject->Render2D();
	}
	for (auto service : mServices)
	{
		service->Render2D();
	}
}

void World::DestroyInternal(GameObjectHandle handle)
{
	ASSERT(!mUpdating, "[World] Cannot destroy game objact during update");

	if (!handle.IsValid())
	{
		return;
	}

	GameObject* gameObject = handle.Get();

	auto iter = std::find(mUpdateList.begin(), mUpdateList.end(), gameObject);
	if (iter != mUpdateList.end())
	{
		mUpdateList.erase(iter);
	}

	// terminat game object
	gameObject->Terminate();

	// unregister from handle pool
	mGameObjectHandlePool->Unregister(handle);

	// destroy from game object factory
	mGameObjectFactory->Destroy(gameObject);
}

void World::PurgeDestroyList()
{
	for (auto handle : mDestroyList)
	{
		DestroyInternal(handle);
	}
	mDestroyList.clear();
}