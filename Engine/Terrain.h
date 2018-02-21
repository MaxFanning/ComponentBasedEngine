#ifndef INCLUDED_ENGINE_TERRAIN_H
#define INCLUDED_ENGINE_TERRAIN_H

//====================================================================================================
// Filename:	Terrain.h
// Created by:	Peter Chan
// Description:	Class for rendering a terrain.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ConstantBuffer.h"
#include "Heightmap.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Camera;
class CameraComponent;
class GraphicsSystem;
class Texture;

//====================================================================================================
// Class Declarations
//====================================================================================================

class Terrain
{
public:
	static Terrain* main;

	Terrain();
	~Terrain();

	void Initialize(const char* pFilename, u32 width, u32 length, f32 maxHeight);
	void Terminate();

	void SetLayer(const char* pFilename, u32 layer, f32 minHeight, f32 maxHeight, f32 frequency);

	bool Intersect(const Math::Ray& ray, f32& distance) const;
	f32 GetHeight(const Math::Vector3& position) const;
	Heightmap& const GetHeightMap() { return mHeightmap; }

	void Render();
	void DebugRender();

private:
	NONCOPYABLE(Terrain);

	static const u32 kMaxTerrainLayers = 4;

	struct CBuffer
	{
		XMMATRIX matWVP;
		XMVECTOR layer[kMaxTerrainLayers];
	};
	
	TypedConstantBuffer<CBuffer> mCBuffer;

	VertexShader mVertexShader;
	PixelShader mPixelShader;

	Heightmap mHeightmap;
	Sectors mTerrainSectors;
	Mesh mTerrainMesh;
	MeshBuffer mTerrain;

	Texture* mpTerrainLayers[kMaxTerrainLayers];
	Math::Vector3 mLayerInfo[kMaxTerrainLayers];

	Sampler mSampler;

	bool mInitialized;
};

#endif // #ifndef INCLUDED_ENGINE_TERRAIN_H