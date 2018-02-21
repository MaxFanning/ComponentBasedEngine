#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "GameObject.h"

class GameObjectFactory
{
public:
	typedef std::function<Component*(const TiXmlNode*)> CreateFunc;

	GameObjectFactory(GameObjectAllocator& gAllocator);

	void Register(const char* name, CreateFunc component);

	GameObject* Create(const char* templateFilename);
	void Destroy(GameObject* newObject);

private:
	typedef std::unordered_map<std::string, CreateFunc> CreateFuncMap;

	GameObjectAllocator& mGameObjectAllocator;
	CreateFuncMap mCreateFuncMap;
};

#endif // !GAME_OBJECT_FACTORY_H