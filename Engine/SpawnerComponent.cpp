#include "Precompiled.h"

#include "SpawnerComponent.h"
#include "BasicBulletComponent.h"
#include "MinionLocationService.h"
#include "XMLReader.h"

Component* SpawnerComponent::CreateFunc(const TiXmlNode* node)
{
	SpawnerComponent* newComponent = new SpawnerComponent; //do an allocation
	const TiXmlNode* data = node->FirstChild();

	// Directory
	const char* direc = XMLReader::ReadString(data);
	newComponent->SetPrefabDirectory(const_cast<char*>(direc));

	// SpawnDelay
	data = data->NextSibling();
	const f32 delay = XMLReader::ReadFloat(data);
	newComponent->SetSpawnDelay(delay);

	return newComponent;
}

SpawnerComponent::SpawnerComponent()
	: mWorld(nullptr)
	, mPrefabDirectory(NULL)
	, mPrefabName("Minion ")
	, mSpawnDelay(0.0f)
	, mTimer(0.0f)
	, mPrefabID(0)
	, mOriginalSpawnDelay(0.0f)
{
}

SpawnerComponent::~SpawnerComponent()
{
	delete[] mPrefabDirectory;
}

void SpawnerComponent::Initialize()
{
	mWorld = &GetWorld();
}

void SpawnerComponent::Update(f32 deltaTime)
{
	Spawn(deltaTime);
}

void SpawnerComponent::Spawn(f32 deltaTime)
{
	mTimer += deltaTime;

	if (mTimer >= mSpawnDelay)
	{
		mSpawnDelay += mOriginalSpawnDelay;
		std::string name = mPrefabName + std::to_string(mPrefabID);
		const char* charName = name.c_str();
		GameObject* gameObject = (mWorld->Create(mPrefabDirectory, charName)).Get();
		++mPrefabID;

		if (gameObject->GetComponent<TransformComponent>())
		{
			gameObject->GetComponent<TransformComponent>()->SetPosition(GetObj().GetComponent<TransformComponent>()->GetPosition());
		}
		if (gameObject->GetComponent<MinionComponent>())
		{
			MinionLocationService::main->AddMinion(gameObject);
		}
	}
}

void SpawnerComponent::SetPrefabDirectory(char* direc)
{
	s32 len = std::strlen(direc);
	mPrefabDirectory = new char[len + 1];
	std::strcpy(mPrefabDirectory, direc);
}