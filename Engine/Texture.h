#ifndef INCLUDED_ENGINE_TEXTURE_H
#define INCLUDED_ENGINE_TEXTURE_H

//====================================================================================================
// Filename:	Texture.h
// Created by:	Peter Chan
// Description:	Class for a texture resource.
//====================================================================================================

//====================================================================================================
// Forward Declarations
//====================================================================================================

class GraphicsSystem;

//====================================================================================================
// Class Declarations
//====================================================================================================

class Texture
{
public:
	Texture();
	~Texture();
	
	bool Initialize(const char* pFilename);
	bool Initialize(const wchar_t* pFilename);
	void Terminate();
	
	void BindVS(u32 index);
	void BindPS(u32 index);

private:
	friend class SpriteRenderer;

	ID3D11ShaderResourceView* mpTexture;
};

#endif // #ifndef INCLUDED_ENGINE_TEXTURE_H