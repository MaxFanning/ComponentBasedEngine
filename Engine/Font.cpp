//====================================================================================================
// Filename:	Font.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Font.h"

#include "GraphicsSystem.h"
#include "SpriteRenderer.h"
#include <DirectXTK/Inc/SpriteFont.h>

//====================================================================================================
// Class Definitions
//====================================================================================================

Font::Font()
	: mpSpriteFont(nullptr)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

Font::~Font()
{
	ASSERT(mpSpriteFont == nullptr, "[Font] Font not freed.");
}

//----------------------------------------------------------------------------------------------------
void Font::Initialize(const wchar_t* pFilename)
{
	ASSERT(mpSpriteFont == nullptr, "[Font] Already initialized.");
	mpSpriteFont = new SpriteFont(GraphicsSystem::Get()->GetDevice(), pFilename);
}

//----------------------------------------------------------------------------------------------------
void Font::Terminate()
{
	ASSERT(mpSpriteFont != nullptr, "[Font] Already terminated.");
	SafeDelete(mpSpriteFont);
}

//----------------------------------------------------------------------------------------------------
void Font::Render(const wchar_t* str, u32 x, u32 y)
{
	SpriteRenderer* renderer = SpriteRenderer::Get();
	ASSERT(mpSpriteFont != nullptr, "[Font] Not initialized.");
	ASSERT(renderer->mpSpriteBatch != nullptr, "[Font] Renderer not initialized.");
	mpSpriteFont->DrawString(renderer->mpSpriteBatch, str, XMFLOAT2((f32)x, (f32)y));
}