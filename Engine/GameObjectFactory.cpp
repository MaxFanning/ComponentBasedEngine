#include "Precompiled.h"
#include "GameObjectFactory.h"

#include <Engine.h>
#include <Utils.h>
#include "XMLReader.h"

GameObjectFactory::GameObjectFactory(GameObjectAllocator& gAllocator)
	: mGameObjectAllocator(gAllocator)
{
}

void GameObjectFactory::Register(const char* name, CreateFunc component)
{
	mCreateFuncMap.insert({ name, component });
}

GameObject* GameObjectFactory::Create(const char* templateFilename)
{
	TiXmlDocument document(templateFilename);

	if (!document.LoadFile())
	{
		LOG("[GameObjectFactory] Failed to open template file '%s'.", templateFilename);
		return nullptr;
	}
	GameObject* gameObject = mGameObjectAllocator.New();
	
	const TiXmlNode* gameObjectNode = document.FirstChild();
	if (Utils::AreEqual(gameObjectNode->Value(), "GameObject")) //if the node is a gameobject 
	{
		const TiXmlNode* componentNode = gameObjectNode->FirstChild();
		if (Utils::AreEqual(componentNode->Value(), "Components")) //if node is the node with the components
		{
			const TiXmlNode* nextComponentNode = componentNode->FirstChild();

			while (nextComponentNode != nullptr)
			{
				const char* componentName = nextComponentNode->Value();
				auto function = mCreateFuncMap[componentName];

				Component* newComponent = function(nextComponentNode);
				gameObject->AddComponent(newComponent);

				nextComponentNode = nextComponentNode->NextSibling();
			}
		}
	}

	return gameObject;
}

void GameObjectFactory::Destroy(GameObject* newObject)
{
	mGameObjectAllocator.Delete(newObject);
}