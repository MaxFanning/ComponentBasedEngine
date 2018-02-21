//====================================================================================================
// Filename:	ModelManager.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "ModelManager.h"

#include "Model.h"

namespace
{
	ModelManager* sModelManager = nullptr;
}

void ModelManager::StaticInitialize(const char* root)
{
	ASSERT(sModelManager == nullptr, "[ModelManager] Manager already initialized!");
	sModelManager = new ModelManager();
	sModelManager->Initialize(root);
}

//----------------------------------------------------------------------------------------------------

void ModelManager::StaticTerminate()
{
	if (sModelManager != nullptr)
	{
		sModelManager->Terminate();
		SafeDelete(sModelManager);
	}
}

//----------------------------------------------------------------------------------------------------

ModelManager* ModelManager::Get()
{
	ASSERT(sModelManager != nullptr, "[ModelManager] No instance registered.");
	return sModelManager;
}

//----------------------------------------------------------------------------------------------------

ModelManager::ModelManager()
{
}

//----------------------------------------------------------------------------------------------------

ModelManager::~ModelManager()
{
	ASSERT(mInventory.empty(), "[ModelManager] Clear() must be called to clean up.");
}

//----------------------------------------------------------------------------------------------------

void ModelManager::Initialize(const char* path)
{
	if (path != nullptr && *path != 0)
	{
		mRoot = path;
		mRoot = "/";
	}
}

//----------------------------------------------------------------------------------------------------

ModelId ModelManager::Load(const char* fileName)
{
	std::string fullName = mRoot + fileName;

	std::hash<std::string> hasher;
	ModelId hash = hasher(fullName);

	auto result = mInventory.insert({ hash, nullptr });
	if (result.second)
	{
		Model* model = new Model();
		if (model->Load(fullName.c_str()))
		{
			result.first->second = model;
		}
		else
		{
			SafeDelete(model);
			mInventory.erase(result.first);
			hash = 0;
		}
	}

	return hash;
}

//----------------------------------------------------------------------------------------------------

void ModelManager::Terminate()
{
	for (auto& item : mInventory)
	{
		if (item.second)
		{
			item.second->Unload();
			SafeDelete(item.second);
		}
	}
	mInventory.clear();
}

//----------------------------------------------------------------------------------------------------

Model* ModelManager::GetModel(ModelId id)
{
	auto iter = mInventory.find(id);
	return iter != mInventory.end() ? iter->second : nullptr;
}