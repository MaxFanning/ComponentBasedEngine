//====================================================================================================
// Filename:	Texture.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Texture.h"

#include "GraphicsSystem.h"
#include <DirectXTK/Inc/DDSTextureLoader.h>
#include <DirectXTK/Inc/WICTextureLoader.h>

//====================================================================================================
// Class Definitions
//====================================================================================================

Texture::Texture()
	: mpTexture(nullptr)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------
Texture::~Texture()
{
	ASSERT(mpTexture == nullptr, "[Texture] Texture not released!");
}

//----------------------------------------------------------------------------------------------------

bool Texture::Initialize(const char* pFilename)
{
	wchar_t buffer[256];
	mbstowcs_s(nullptr, buffer, 256, pFilename, 256);
	return Initialize(buffer);
}

//----------------------------------------------------------------------------------------------------

bool Texture::Initialize(const wchar_t* pFilename)
{
	GraphicsSystem* gs = GraphicsSystem::Get();

	CreateDDSTextureFromFile(gs->GetDevice(), pFilename, nullptr, &mpTexture);
	if (mpTexture == nullptr)
	{
		CreateWICTextureFromFile(gs->GetDevice(), gs->GetContext(), pFilename, nullptr, &mpTexture);
	}
	return mpTexture != nullptr;
}

//----------------------------------------------------------------------------------------------------

void Texture::Terminate()
{
	SafeRelease(mpTexture);
}

//----------------------------------------------------------------------------------------------------

void Texture::BindVS(u32 index)
{
	GraphicsSystem::Get()->GetContext()->VSSetShaderResources(index, 1, &mpTexture);
}

//----------------------------------------------------------------------------------------------------

void Texture::BindPS(u32 index)
{
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(index, 1, &mpTexture);
}