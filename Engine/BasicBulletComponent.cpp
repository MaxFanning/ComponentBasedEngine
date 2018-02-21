#include "Precompiled.h"

#include "BasicBulletComponent.h"
#include "MinionLocationService.h"
#include "XMLReader.h"

Component* BasicBulletComponent::CreateFunc(const TiXmlNode* node)
{
	BasicBulletComponent* newComponent = new BasicBulletComponent; //do an allocation
	const TiXmlNode* data = node->FirstChild();

	// Speed
	const f32 speed = XMLReader::ReadFloat(data);
	newComponent->SetSpeed(speed);

	// Life Span
	data = data->NextSibling();
	const f32 lifespan = XMLReader::ReadFloat(data);
	newComponent->SetLifeSpan(lifespan);

	return newComponent;
}

BasicBulletComponent::BasicBulletComponent()
	: mPosition(Math::Vector3::Zero())
	, mSpeed(NULL)
	, mRange(1.5f)
	, mForwardVector(Math::Vector3::Zero())
	, mTransform(nullptr)
	, mLifeSpan(0.0f)
	, mTimer(0.0f)
{
}

BasicBulletComponent::~BasicBulletComponent()
{
}

void BasicBulletComponent::Initialize()
{
	mTransform = GetObj().GetComponent<TransformComponent>();
}

void BasicBulletComponent::Update(f32 deltaTime)
{
	mTimer += deltaTime;
	if (mTimer >= mLifeSpan)
	{
		MinionLocationService::main->RemoveFromList(this->GetObj());
		GetWorld().Destroy(this->GetObj().GetHandle());
	}

	mPosition = mTransform->GetPosition();

	//if (Math::Distance(mPosition, targetPos) < mRange)
	//{
	//	GetWorld().Destroy(mTarget->GetHandle());
	//	GetWorld().Destroy(this->GetObj().GetHandle());
	//	return;
	//}

	mPosition += (mForwardVector * mSpeed * deltaTime);

	mTransform->SetPosition(mPosition);
}

void BasicBulletComponent::SetSpeed(f32 spd)
{
	mSpeed = spd;
}

void BasicBulletComponent::SetDirection(Math::Vector3 forward)
{
	mForwardVector = forward;
}

void BasicBulletComponent::SetLifeSpan(f32 lifespan)
{
	mLifeSpan = lifespan;
}