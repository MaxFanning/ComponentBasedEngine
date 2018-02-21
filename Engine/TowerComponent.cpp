#include "Precompiled.h"

#include "TowerComponent.h"
#include "XMLReader.h"

Component* TowerComponent::CreateFunc(const TiXmlNode* node)
{
	TowerComponent* newComponent = new TowerComponent; //do an allocation
	const TiXmlNode* data = node->FirstChild();

	// Directory
	const char* direc = XMLReader::ReadString(data);
	newComponent->SetPrefabDirectory(const_cast<char*>(direc));

	// SpawnDelay
	data = data->NextSibling();
	const f32 delay = XMLReader::ReadFloat(data);
	newComponent->SetSpawnDelay(delay);

	// Range
	data = data->NextSibling();
	const f32 range = XMLReader::ReadFloat(data);
	newComponent->SetRange(range);

	return newComponent;
}

TowerComponent::TowerComponent()
	: SpawnerComponent()
{
	mRange = 0.0f;
	mPrefabName = "Bullet ";
}

TowerComponent::~TowerComponent()
{
}

void TowerComponent::Initialize()
{
	SpawnerComponent::Initialize();

	// Visitor
	mVisitor.closestGO = nullptr;
	mVisitor.closest = 999999;
	mVisitor.range = mRange;
	mVisitor.towerPosition = GetObj().GetComponent<TransformComponent>()->GetPosition();
	mVisitor.range = mRange;
}

void TowerComponent::Update(f32 deltaTime)
{
	if (mVisitor.closestGO == nullptr)
	{
		mWorld->Visit(mVisitor);
	}

	if (mVisitor.closestGO != nullptr)
	{
		Spawn(deltaTime);
	}
}

void TowerComponent::Spawn(f32 deltaTime)
{
	mTimer += deltaTime;

	if (mTimer >= mSpawnDelay)
	{
		mSpawnDelay += mOriginalSpawnDelay;

		if (mVisitor.closestGO->GetHandle().IsValid()/* == mVisitor.closestGOHandle*/)
		{
			std::string name = mPrefabName + std::to_string(mPrefabID);
			const char* charName = name.c_str();
			GameObject* bullet = (mWorld->Create(mPrefabDirectory, charName)).Get();
			bullet->GetComponent<BulletComponent>()->SetTarget(mVisitor.closestGO);
			++mPrefabID;
		}
	}
}