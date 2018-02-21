#include "Precompiled.h"

#include "FPSCameraControllerComponent.h"
#include "Terrain.h"
#include "TransformComponent.h"
#include "InputTypes.h"
#include "XMLReader.h"

Component* FPSCameraControllerComponent::CreateFunc(const TiXmlNode* node)
{
	FPSCameraControllerComponent* newComponent = new FPSCameraControllerComponent;

	const TiXmlNode* data = node->FirstChild();
	const f32 moveSpeed = XMLReader::ReadFloat(data);

	data = data->NextSibling();
	const f32 turnSpeed = XMLReader::ReadFloat(data);
	newComponent->SetMoveSpeed(moveSpeed);
	newComponent->SetTurnSpeed(turnSpeed);

	data = data->NextSibling();
	const Math::Vector3 offset = XMLReader::ReadVector(data);
	newComponent->SetOffset(offset);

	return newComponent;
}

//----------------------------------------------------------------------------------------------------

FPSCameraControllerComponent::FPSCameraControllerComponent()
	: CameraComponent()
{
	mInputSystem = InputSystem::Get();
	mMoveSpeed = 0.0f;
	mTurnSpeed = 0.0f;
	mTransform = nullptr;
	mTerrain = nullptr;
	mWidth = NULL;
	mLength = NULL;
}

//----------------------------------------------------------------------------------------------------

FPSCameraControllerComponent::~FPSCameraControllerComponent()
{
	
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::Update(f32 deltaTime)
{
	if (mWidth == 0.0f || mLength == 0.0f)
	{
		mWidth = (float)Terrain::main->GetHeightMap().GetColumns();
		mLength = (float)Terrain::main->GetHeightMap().GetRows();
	}

	if (mInputSystem->IsKeyDown(Keys::W))
	{
		Walk(deltaTime);
		//mPosition.z += (mMoveSpeed * deltaTime);
	}
	if (mInputSystem->IsKeyDown(Keys::S))
	{
		Walk(-deltaTime);
		//mPosition.z -= (mMoveSpeed * deltaTime);
	}
	if (mInputSystem->IsKeyDown(Keys::D))
	{
		Strafe(deltaTime);
		//mPosition.x += (mMoveSpeed * deltaTime);
	}
	if (mInputSystem->IsKeyDown(Keys::A))
	{
		Strafe(-deltaTime);
		//mPosition.x -= (mMoveSpeed * deltaTime);
	}

	Yaw(mInputSystem->GetMouseMoveX(), deltaTime);
	Pitch(mInputSystem->GetMouseMoveY(), deltaTime);
	mPosition.y = mTerrain->GetHeight(mPosition);

	mTransform->SetPosition(mPosition);
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::Initialize()
{
	mTransform = GetObj().GetComponent<TransformComponent>();
	mTerrain = Terrain::main;
	//mWidth = (float)Terrain::main->GetHeightMap().GetColumns();
	//mLength = (float)Terrain::main->GetHeightMap().GetRows();
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::Render()
{

}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::SetDirection(const Math::Vector3& direction)
{
	mLook = direction;
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::SetLookAt(const Math::Vector3& target)
{
	mLook = target - mPosition;
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::SetMoveSpeed(const f32& speed)
{
	mMoveSpeed = speed;
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::SetTurnSpeed(const f32& speed)
{
	mTurnSpeed = speed;
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::Walk(f32 deltaTime)
{
	mPosition = mPosition + (mLook * (mMoveSpeed * deltaTime));
	PositionCheck();
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::Strafe(f32 deltaTime)
{
	mPosition = mPosition + (mRight * (mMoveSpeed * deltaTime));
	PositionCheck();
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::Rise(f32 deltaTime)
{
	mPosition = mPosition + (Math::Vector3::YAxis() * (mMoveSpeed * deltaTime));
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::Yaw(s32 direction, f32 deltaTime)
{
	const Math::Matrix matRotate = Math::Matrix::RotationY((direction * deltaTime * mTurnSpeed) * Math::kDegToRad);
	mLook = Math::TransformNormal(mLook, matRotate);
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void FPSCameraControllerComponent::Pitch(s32 direction, f32 deltaTime)
{
	const Math::Matrix matRotate = Math::Matrix::RotationAxis(mRight, (direction * deltaTime * mTurnSpeed) * Math::kDegToRad);
	const Math::Vector3 newLook = Math::TransformNormal(mLook, matRotate);
	const f32 dot = Math::Dot(newLook, Math::Vector3::YAxis());

	// Avoid looking straight up or down
	if (Math::Abs(dot) < 0.995f)
	{
		mLook = newLook;
		Renormalize();
	}
}

void FPSCameraControllerComponent::PositionCheck()
{
	if (mPosition.z > mWidth)
	{
		mPosition.z = mWidth;
	}
	else if (mPosition.z < 0.0f)
	{
		mPosition.z = 0.0f;
	}

	if (mPosition.x > mLength)
	{
		mPosition.x = mWidth;
	}
	else if (mPosition.x < 0.0f)
	{
		mPosition.x = 0.0f;
	}
}