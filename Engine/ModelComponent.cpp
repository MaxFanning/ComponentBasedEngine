#include "Precompiled.h"

#include "ModelComponent.h"
#include "ModelManager.h"
#include "TransformComponent.h"
#include "XMLReader.h"

Component* ModelComponent::CreateFunc(const TiXmlNode* node)
{
	ModelComponent* newComponent = new ModelComponent; //do an allocation
	const TiXmlNode* data = node->FirstChild();

	// Directory
	const char* direc = XMLReader::ReadString(data);
	newComponent->SetModelDirectory(const_cast<char*>(direc));

	// Scale
	data = data->NextSibling();
	const f32 scale = XMLReader::ReadFloat(data);
	newComponent->SetScale(scale);

	// Offset
	data = data->NextSibling();
	const Math::Vector3 offset = XMLReader::ReadVector(data);
	newComponent->SetOffset(offset);

	// Rotation Y
	data = data->NextSibling();
	const float rot = XMLReader::ReadFloat(data);
	newComponent->SetRotY(rot);

	return newComponent;
}

ModelComponent::ModelComponent()
	: mModelDirectory(nullptr)
	, mTransform(nullptr)
	, mScale(0.0f)
	, mOffset(Math::Vector3::Zero())
	, mRotationY(0.0f)
	, mModel(nullptr)
{
}

ModelComponent::~ModelComponent()
{
}

void ModelComponent::Initialize()
{
	ASSERT(mModelDirectory != nullptr, "[ModelComponent] Model is NULL.");

	if (CameraComponent::main)
	{
		u32 id = ModelManager::Get()->Load(mModelDirectory);
		mModel = ModelManager::Get()->GetModel(id);
	}
	mTransform = GetObj().GetComponent<TransformComponent>();
}

void ModelComponent::Terminate()
{
	mModel = nullptr;
}

void ModelComponent::Render()
{
	Math::Vector3 pos = mTransform->GetPosition();
	Math::Matrix matrix;
	matrix = matrix.Scaling(mScale) + matrix.Translation(pos + mOffset);
	matrix._44 = 1.0f;

	Math::Matrix rotMatrix;
	rotMatrix = rotMatrix.RotationY(mRotationY);
	matrix = Math::Inverse(Math::Inverse(matrix) * rotMatrix);
	//matrix._11 = rotMatrix._11; matrix._13 = rotMatrix._13; matrix._31 = rotMatrix._31; matrix._33 = rotMatrix._33;

	mModel->Render(matrix);
	//mModel.Render(Math::Matrix::Translation(pos + mOffset)); // TODO: Make variable for offset later
}

void ModelComponent::SetModelDirectory(char* direc)
{
	s32 len = std::strlen(direc);
	mModelDirectory = new char[len + 1];
	std::strcpy(mModelDirectory, direc);
}

void ModelComponent::SetScale(const f32& scale)
{
	mScale = scale;
}

void ModelComponent::SetOffset(const Math::Vector3& offset)
{
	mOffset = offset;
}

void ModelComponent::SetRotY(const float& rotY)
{
	mRotationY = rotY;
}