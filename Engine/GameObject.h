#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "HandlePool.h"
#include "TypedAllocator.h"

class Component;
class GameObject;
class World;

typedef TypedAllocator<GameObject> GameObjectAllocator;
typedef Handle<GameObject> GameObjectHandle;
typedef HandlePool<GameObject> GameObjectHandlePool;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Initialize();
	void Terminate();

	void Update(f32 deltaTime);
	void Render();
	void Render2D();

	void AddComponent(Component* component);

	template <typename T>
	T* GetComponent();
	template <typename T>
	const T* GetComponent() const;

	World& GetWorld() { return *mWorld; }
	const char* GetName() const { return mName.c_str(); }
	GameObjectHandle GetHandle() const { return mHandle; }

private:
	friend class World;

	typedef std::vector<Component*> Components;

	World* mWorld;
	std::string mName;
	GameObjectHandle mHandle;
	Components mComponents;
};

template <typename T>
T* GameObject::GetComponent()
{
	const GameObject* constMe = this;
	return const_cast<T*>(constMe->GetComponent<T>());
}

template <typename T>
const T* GameObject::GetComponent() const
{
	for (auto component : mComponents)
	{
		if (component->GetType() == T::StaticGetType())
		{
			return static_cast<const T*>(component);
		}
	}
	return nullptr;
}

#endif // !GAME_OBJECT_H