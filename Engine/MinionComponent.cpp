#include "Precompiled.h"

#include "MinionComponent.h"
#include "MinionLocationService.h"
#include "Terrain.h"
#include "GameObject.h"
#include "XMLReader.h"
#include <time.h>

Component* MinionComponent::CreateFunc(const TiXmlNode* node)
{
	MinionComponent* newComponent = new MinionComponent; //do an allocation
	const TiXmlNode* data = node->FirstChild();

	// Destination
	const Math::Vector3 dest = XMLReader::ReadVector(data);
	newComponent->SetDestination(dest);

	// Speed
	data = data->NextSibling();
	const f32 speed = XMLReader::ReadFloat(data);
	newComponent->SetSpeed(speed);

	// LifeSpawn
	data = data->NextSibling();
	const f32 lifeSpan = XMLReader::ReadFloat(data);
	newComponent->SetLifeSpan(lifeSpan);

	return newComponent;
}

MinionComponent::MinionComponent()
	: mDestination(Math::Vector3::Zero())
	, mSpeed(0.0f)
	, mPosition(Math::Vector3::Zero())
	, mTransform(nullptr)
	, mAliveTimer(0.0f)
	, mLifeSpan(NULL)
	, mTerrain(nullptr)
	, mOriginalY(0.0f)
{
}

MinionComponent::~MinionComponent()
{
}

void MinionComponent::Initialize()
{
	srand(time(NULL));
	mTransform = GetObj().GetComponent<TransformComponent>();
	mTerrain = Terrain::main;
	mPosition = mTransform->GetPosition();
	mOriginalY = mDestination.y;
}

void MinionComponent::Update(f32 deltaTime)
{
	mAliveTimer += deltaTime;
	if (mAliveTimer >= mLifeSpan)
	{
		MinionLocationService::main->RemoveFromList(this->GetObj());
		GetWorld().Destroy(this->GetObj().GetHandle());
	}

	mPosition = mTransform->GetPosition();
	if (Math::Distance(mPosition, mDestination) <= 10.0f)
	{
		mDestination.x = rand() % mTerrain->GetHeightMap().GetColumns();
		mDestination.z = rand() % mTerrain->GetHeightMap().GetRows();
		mDestination.y = rand() % (int)mOriginalY + 1;
	}

	if (mPosition.x < mDestination.x)
	{
		mPosition.x += (mSpeed * deltaTime);
	}
	if (mPosition.x > mDestination.x)
	{
		mPosition.x -= (mSpeed * deltaTime);
	}
	if (mPosition.y < mDestination.y)
	{
		mPosition.y += (mSpeed * deltaTime);
	}
	if (mPosition.y > mDestination.y)
	{
		mPosition.y -= (mSpeed * deltaTime);
	}
	if (mPosition.z < mDestination.z)
	{
		mPosition.z += (mSpeed * deltaTime);
	}
	if (mPosition.z > mDestination.z)
	{
		mPosition.z -= (mSpeed * deltaTime);
	}

	mTransform->SetPosition(mPosition);
}

void MinionComponent::SetDestination(Math::Vector3 dest)
{
	mDestination = dest;
}

void MinionComponent::SetSpeed(f32 spd)
{
	mSpeed = spd;
}

void MinionComponent::SetLifeSpan(f32 span)
{
	mLifeSpan = span;
}