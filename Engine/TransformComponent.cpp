#include "Precompiled.h"

#include "TransformComponent.h"

#include "XMLReader.h"
#include "GameObject.h"
#include "Terrain.h"

Component* TransformComponent::CreateFunc(const TiXmlNode* node)
{
	TransformComponent* newComponent = new TransformComponent; //do an allocation
	const TiXmlNode* data = node->FirstChild();
	const Math::Vector3 pos = XMLReader::ReadVector(data);
	newComponent->SetPosition(pos);
	return newComponent;
}

TransformComponent::TransformComponent()
	: mPosition(Math::Vector3::Zero())
	, mForward(Math::Vector3::ZAxis())
	, mSnapToTerrain(false)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Update(f32 deltaTime)
{
	if (mSnapToTerrain)
	{
		mPosition.y = Terrain::main->GetHeight(mPosition);
	}
}

Math::Matrix TransformComponent::GetLocalToWorld() const
{
	Math::Vector3 yAxis = Math::Vector3::YAxis();
	Math::Vector3 forward = Math::Normalize(mForward);
	Math::Vector3 side = Math::Cross(yAxis, forward);
	Math::Vector3 up = Math::Cross(forward, side);
	Math::Vector3 pos = mPosition;

	return Math::Matrix
	(
		side.x, side.y, side.z, 0.0f,
		up.x, up.y, up.z, 0.0f,
		forward.x, forward.y, forward.z, 0.0f,
		pos.x, pos.y, pos.z, 1.0f
	);
}