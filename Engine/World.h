#ifndef INCLUDED_ENGINE_WORLD_H
#define INCLUDED_ENGINE_WORLD_H

#include "GameObjectFactory.h"
#include "HandlePool.h"

struct Visitor
{
	virtual void operator()(GameObject* go) = 0;
};

class Service;
class ServiceFactory;

typedef TypedAllocator<Service> ServiceAllocator;

class World
{
public:
	World();
	~World();

	void Intialize(u32 capacity);
	void Terminate();

	void LoadLevel(const char* level);

	GameObjectHandle Create(const char* templateFileName, const char* name);
	GameObjectHandle Find(const char* name);
	void Destroy(GameObjectHandle handle);

	void AddService(Service* service);
	template <typename T>
	T* GetService();
	template <typename T>
	const T* GetService() const;

	void Visit(Visitor& v);

	void Update(f32 deltaTime);
	void Render();
	void Render2D();

private:
	void DestroyInternal(GameObjectHandle handle);
	void PurgeDestroyList();

	typedef std::vector<GameObject*> GameObjectList;
	typedef std::vector<GameObjectHandle> DestroyList;
	typedef std::vector<Service*> Services;

	std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
	std::unique_ptr<ServiceAllocator> mServiceAllocator;
	std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;
	std::unique_ptr<GameObjectFactory> mGameObjectFactory;
	std::unique_ptr<ServiceFactory> mServiceFactory;

	GameObjectList mUpdateList;
	DestroyList mDestroyList;
	Services mServices;
	bool mUpdating;
};

template <typename T>
T* World::GetService()
{
	const World* constMe = this;
	return const_cast<T*>(constMe->GetService<T>());
}

template <typename T>
const T* World::GetService() const
{
	for (auto service : mServices)
	{
		if (service->GetType() == T::StaticGetType())
		{
			return static_cast<const T*>(service);
		}
	}
	return nullptr;
}

#endif