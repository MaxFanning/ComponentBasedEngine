#include "Precompiled.h"

#include "TurretComponent.h"
#include "BasicBulletComponent.h"
#include "MinionLocationService.h"
#include "XMLReader.h"

Component* TurretComponent::CreateFunc(const TiXmlNode* node)
{
	TurretComponent* newComponent = new TurretComponent; //do an allocation
	const TiXmlNode* data = node->FirstChild();

	// Directory
	const char* direc = XMLReader::ReadString(data);
	newComponent->SetPrefabDirectory(const_cast<char*>(direc));

	return newComponent;
}

TurretComponent::TurretComponent()
	: SpawnerComponent()
{
}

TurretComponent::~TurretComponent()
{
}

void TurretComponent::Update(f32 deltaTime)
{
	if (InputSystem::Get()->IsMousePressed(Mouse::LBUTTON))
	{
		Spawn();
	}
}

void TurretComponent::Spawn()
{
	std::string name = "Bullet " + std::to_string(mPrefabID);
	const char* charName = name.c_str();
	GameObject* gameObject = (mWorld->Create(mPrefabDirectory, charName)).Get();
	++mPrefabID;

	if (gameObject->GetComponent<BasicBulletComponent>())
	{
		gameObject->GetComponent<BasicBulletComponent>()->SetDirection(CameraComponent::main->GetLook());
		Math::Vector3 cameraView = CameraComponent::main->GetPosition() + CameraComponent::main->GetOffset();
		gameObject->GetComponent<TransformComponent>()->SetPosition(cameraView);
		MinionLocationService::main->AddBullet(gameObject);
	}
}