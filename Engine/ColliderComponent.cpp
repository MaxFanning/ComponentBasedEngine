#include "Precompiled.h"

#include "ColliderComponent.h"

#include "GameObject.h"
#include "SimpleDraw.h"
#include "TransformComponent.h"
#include "XMLReader.h"

Component* ColliderComponent::CreateFunc(const TiXmlNode* node)
{
	ColliderComponent* newComponent = new ColliderComponent; //do an allocation
	const TiXmlNode* data = node->FirstChild();

	// Center
	const Math::Vector3 cent = XMLReader::ReadVector(data);
	newComponent->SetCenter(cent);

	// Extend
	data = data->NextSibling();
	const Math::Vector3 ext = XMLReader::ReadVector(data);
	newComponent->SetExtend(ext);

	// Colour
	data = data->NextSibling();
	const Color color = XMLReader::ReadColor(data);
	newComponent->SetColor(color);

	return newComponent;
}

ColliderComponent::ColliderComponent()
	: mTransformComponent(nullptr)
	, mCenter(Math::Vector3::Zero())
	, mExtend(Math::Vector3::One())
	, mColor(Color::Green())
{
}

ColliderComponent::~ColliderComponent()
{
}

void ColliderComponent::Initialize()
{
	mTransformComponent = GetObj().GetComponent<TransformComponent>();
}

void ColliderComponent::Render()
{
	SimpleDraw::AddAABB(GetAABB(), mColor);
}

Math::AABB ColliderComponent::GetAABB() const
{
	Math::Vector3 pos = mTransformComponent->GetPosition();
	return Math::AABB(mCenter + pos, mExtend);
}