//====================================================================================================
// Filename:	TextureManager.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "TextureManager.h"

#include "Texture.h"

namespace
{
	TextureManager* sTextureManager = nullptr;
}

void TextureManager::StaticInitialize(const char* root)
{
	ASSERT(sTextureManager == nullptr, "[TextureManager] Manager already initialized!");
	sTextureManager = new TextureManager();
	sTextureManager->Initialize(root);
}

//----------------------------------------------------------------------------------------------------

void TextureManager::StaticTerminate()
{
	if (sTextureManager != nullptr)
	{
		sTextureManager->Terminate();
		SafeDelete(sTextureManager);
	}
}

//----------------------------------------------------------------------------------------------------

TextureManager* TextureManager::Get()
{
	ASSERT(sTextureManager != nullptr, "[TextureManager] No instance registered.");
	return sTextureManager;
}

//----------------------------------------------------------------------------------------------------

TextureManager::TextureManager()
{
}

//----------------------------------------------------------------------------------------------------

TextureManager::~TextureManager()
{
	ASSERT(mInventory.empty(), "[TextureManager] Clear() must be called to clean up.");
}

//----------------------------------------------------------------------------------------------------

void TextureManager::Initialize(const char* path)
{
	if (path != nullptr && *path != 0)
	{
		mRoot = path;
		mRoot = "/";
	}
}

//----------------------------------------------------------------------------------------------------

TextureId TextureManager::Load(const char* fileName)
{
	std::string fullName = mRoot + fileName;

	std::hash<std::string> hasher;
	TextureId hash = hasher(fullName);

	auto result = mInventory.insert({ hash, nullptr });
	if (result.second)
	{
		Texture* texture = new Texture();
		if (texture->Initialize(fullName.c_str()))
		{
			result.first->second = texture;
		}
		else
		{
			SafeDelete(texture);
			mInventory.erase(result.first);
			hash = 0;
		}
	}

	return hash;
}

//----------------------------------------------------------------------------------------------------

void TextureManager::Terminate()
{
	for (auto& item : mInventory)
	{
		if (item.second)
		{
			item.second->Terminate();
			SafeDelete(item.second);
		}
	}
	mInventory.clear();
}

//----------------------------------------------------------------------------------------------------

Texture* TextureManager::GetTexture(TextureId id)
{
	auto iter = mInventory.find(id);
	return iter != mInventory.end() ? iter->second : nullptr;
}