#ifndef INCLUDED_ENGINE_SPRITERENDERER_H
#define INCLUDED_ENGINE_SPRITERENDERER_H

//====================================================================================================
// Filename:	SpriteRenderer.h
// Created by:	Peter Chan
// Description:	Class for 2D sprite rendering.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

namespace DirectX
{
	class CommonStates;
	class SpriteBatch;
}
class GraphicsSystem;
class Texture;

//====================================================================================================
// Class Declarations
//====================================================================================================

class SpriteRenderer
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static SpriteRenderer* Get();

public:
	SpriteRenderer();
	~SpriteRenderer();

	void Initialize();
	void Terminate();

	void BeginRender();
	void EndRender();

	void Draw(Texture& texture, const Math::Vector2& position);

	SpriteBatch* GetSpriteBatch() { return mpSpriteBatch; }

private:
	NONCOPYABLE(SpriteRenderer);

	friend class Font;
	
	CommonStates* mpStateObjects;
	SpriteBatch* mpSpriteBatch;
};

#endif // #ifndef INCLUDED_ENGINE_SPRITERENDERER_H