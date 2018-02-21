#include "Precompiled.h"

#include "BulletComponent.h"
#include "XMLReader.h"

Component* BulletComponent::CreateFunc(const TiXmlNode* node)
{
	BulletComponent* newComponent = new BulletComponent; //do an allocation
	const TiXmlNode* data = node->FirstChild();

	// Speed
	const f32 speed = XMLReader::ReadFloat(data);
	newComponent->SetSpeed(speed);

	return newComponent;
}

BulletComponent::BulletComponent()
	: mTarget(nullptr)
	, mPosition(Math::Vector3::Zero())
	, mSpeed(NULL)
	, mTransform(nullptr)
	, mTargetTransform(nullptr)
	, mRange(1.0f)
{
}

BulletComponent::~BulletComponent()
{
}

void BulletComponent::Initialize()
{
	mTransform = GetObj().GetComponent<TransformComponent>();
}

void BulletComponent::Update(f32 deltaTime)
{
	if (mTargetTransform == nullptr && mTarget != nullptr)
	{
		mTargetTransform = mTarget->GetComponent<TransformComponent>();
	}

	if (mTargetTransform != nullptr)
	{
		mPosition = mTransform->GetPosition();
		Math::Vector3 targetPos = mTargetTransform->GetPosition();

		if (Math::Distance(mPosition, targetPos) < mRange)
		{
			GetWorld().Destroy(mTarget->GetHandle());
			GetWorld().Destroy(this->GetObj().GetHandle());
			return;
		}

		if (mPosition.x < targetPos.x)
		{
			mPosition.x += (mSpeed * deltaTime);
		}
		if (mPosition.x > targetPos.x)
		{
			mPosition.x -= (mSpeed * deltaTime);
		}
		if (mPosition.y < targetPos.y)
		{
			mPosition.y += (mSpeed * deltaTime);
		}
		if (mPosition.y > targetPos.y)
		{
			mPosition.y -= (mSpeed * deltaTime);
		}
		if (mPosition.z < targetPos.z)
		{
			mPosition.z += (mSpeed * deltaTime);
		}
		if (mPosition.z > targetPos.z)
		{
			mPosition.z -= (mSpeed * deltaTime);
		}

		mTransform->SetPosition(mPosition);
	}
}

void BulletComponent::SetTarget(GameObject* targ)
{
	mTarget = targ;
}

void BulletComponent::SetSpeed(f32 spd)
{
	mSpeed = spd;
}