//====================================================================================================
// Filename:	Terrain.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Terrain.h"

#include "CameraComponent.h"
#include "GraphicsSystem.h"
#include "MeshBuilder.h"
#include "MeshBuffer.h"
#include "SimpleDraw.h"
#include "Texture.h"

//====================================================================================================
// Statics
//====================================================================================================

Terrain* Terrain::main = nullptr;

//====================================================================================================
// Class Definitions
//====================================================================================================

Terrain::Terrain()
	: mInitialized(false)
{
	for (u32 i = 0; i < kMaxTerrainLayers; ++i)
	{
		mpTerrainLayers[i] = nullptr;
	}
	if (main == nullptr)
	{
		main = this;
	}
}

//----------------------------------------------------------------------------------------------------

Terrain::~Terrain()
{
	ASSERT(!mInitialized, "[Terrain] Not terminated!");

	if (main == this)
	{
		main = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------

void Terrain::Initialize(const char* pFilename, u32 width, u32 length, f32 maxHeight)
{
	ASSERT(!mInitialized, "[Terrain] Already initialized!");
	
	mCBuffer.Initialize();

	const u32 vertexFormat = Mesh::GetVertexFormat();
	mVertexShader.Initialize(L"../Engine/Shaders/Terrain.fx", "VS", "vs_4_0", vertexFormat);
	mPixelShader.Initialize(L"../Engine/Shaders/Terrain.fx", "PS", "ps_4_0");

	mHeightmap.GenerateFromRAW(pFilename, width, length);
	MeshBuilder::CreateTerrain(mTerrainMesh, mTerrainSectors, mHeightmap, maxHeight);
	mTerrain.Initialize(vertexFormat, mTerrainMesh);

	mSampler.Initialize(Sampler::Anisotropic, Sampler::Wrap);

	mInitialized = true;
}

//----------------------------------------------------------------------------------------------------

void Terrain::Terminate()
{
	ASSERT(mInitialized, "[Terrain] Already terminated!");
	
	mCBuffer.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();

	mHeightmap.Purge();
	mTerrain.Terminate();
	mTerrainMesh.Destroy();

	for (u32 i = 0; i < kMaxTerrainLayers; ++i)
	{
		if (mpTerrainLayers[i] != nullptr)
		{
			mpTerrainLayers[i]->Terminate();
			SafeDelete(mpTerrainLayers[i]);
		}
	}

	mSampler.Terminate();

	mInitialized = false;
}

//----------------------------------------------------------------------------------------------------

void Terrain::SetLayer(const char* pFilename, u32 layer, f32 minHeight, f32 maxHeight, f32 frequency)
{
	ASSERT(layer < kMaxTerrainLayers, "[Terrain] Only support %d layers.", kMaxTerrainLayers);
	ASSERT(minHeight <= maxHeight && frequency > 0.0f, "[Terrain] Invalid layer params.");

	if (mpTerrainLayers[layer] != nullptr)
	{
		mpTerrainLayers[layer]->Terminate();
		SafeDelete(mpTerrainLayers[layer]);
	}

	mpTerrainLayers[layer] = new Texture();
	mpTerrainLayers[layer]->Initialize(pFilename);
	mLayerInfo[layer].x = minHeight;
	mLayerInfo[layer].y = maxHeight;
	mLayerInfo[layer].z = frequency;
}

//----------------------------------------------------------------------------------------------------

bool Terrain::Intersect(const Math::Ray& ray, f32& distance) const
{
	distance = FLT_MAX;
	
	const Mesh::Vertex* vertices = mTerrainMesh.GetVertices();
	bool intersect = false;

	// Find the sector the ray is intersecting
	for (auto& sector : mTerrainSectors)
	{
		f32 a, b;
		if (Math::Intersect(ray, sector.aabb, a, b))
		{
			for(u32 z = sector.minZ; z < sector.maxZ; ++z)
			{
				for (u32 x = sector.minX; x < sector.maxX; ++x)
				{
					int a = (x + 0) + ((z + 0) * mHeightmap.GetColumns());
					int b = (x + 0) + ((z + 1) * mHeightmap.GetColumns());
					int c = (x + 1) + ((z + 1) * mHeightmap.GetColumns());
					int d = (x + 1) + ((z + 0) * mHeightmap.GetColumns());

					{
						Math::Vector3 v0 = vertices[a].position;
						Math::Vector3 v1 = vertices[b].position;
						Math::Vector3 v2 = vertices[c].position;

						f32 dist = 0.0f;
						if (Math::Intersect(ray, v0, v1, v2, dist) && dist > 0.0f)
						{
							intersect = true;
							if (dist < distance)
							{
								distance = dist;
							}
						}
					}

					{
						Math::Vector3 v0 = vertices[a].position;
						Math::Vector3 v1 = vertices[c].position;
						Math::Vector3 v2 = vertices[d].position;

						f32 dist = 0.0f;
						if (Math::Intersect(ray, v0, v1, v2, dist) && dist > 0.0f)
						{
							intersect = true;
							if (dist < distance)
							{
								distance = dist;
							}
						}
					}
				}
			}
		}
	}

	return intersect;
}

//----------------------------------------------------------------------------------------------------

f32 Terrain::GetHeight(const Math::Vector3& position) const
{
	const u32 rows = mHeightmap.GetRows();
	const u32 cols = mHeightmap.GetColumns();
	const f32 length = (f32)rows - 1.0f;
	const f32 width = (f32)cols - 1.0f;
	if (position.x < 0.0f || position.x > width ||
		position.z < 0.0f || position.z > length)
	{
		return 0.0f;
	}

	const u32 minX = (u32)position.x;
	const u32 minZ = (u32)position.z;
	const u32 maxX = minX + 1;
	const u32 maxZ = minZ + 1;

	//
	//  B---C
	//  | / |
	//	A---D
	//
	const Mesh::Vertex* vertices = mTerrainMesh.GetVertices();
	const f32 a = vertices[minX + (minZ * cols)].position.y;
	const f32 b = vertices[minX + (maxZ * cols)].position.y;
	const f32 c = vertices[maxX + (maxZ * cols)].position.y;
	const f32 d = vertices[maxX + (minZ * cols)].position.y;

	const f32 deltaX = position.x - (f32)minX;
	const f32 deltaZ = position.z - (f32)minZ;

	f32 height = a;
	if (deltaX > deltaZ)
	{
		height = d + ((1.0f - deltaX) * (a - d)) + (deltaZ * (c - d));
	}
	else
	{
		height = b + ((1.0f - deltaZ) * (a - b)) + (deltaX * (c - b));
	}
	return height;
}

//----------------------------------------------------------------------------------------------------

void Terrain::Render()
{
	if (CameraComponent::main == nullptr)
		return;

	ASSERT(mpTerrainLayers[0] != nullptr, "[Terrain] At least one layer must be set!");

	XMMATRIX matView = *(XMMATRIX*)&(CameraComponent::main->GetViewMatrix());
	XMMATRIX matProj = *(XMMATRIX*)&(CameraComponent::main->GetProjectionMatrix());

	CBuffer cb;
	cb.matWVP = XMMatrixTranspose(matView * matProj);
	for (u32 i = 0; i < kMaxTerrainLayers; ++i)
	{
		if (mpTerrainLayers[i] != nullptr)
		{
			cb.layer[i] = XMLoadFloat3((XMFLOAT3*)&mLayerInfo[i]);
			mpTerrainLayers[i]->BindPS(i);
		}
		else
		{
			cb.layer[i] = XMVectorSet(F32_MAX, F32_MAX, 0.0f, 0.0f);
		}
	}
	mCBuffer.Set(cb);
	mCBuffer.BindVS(0);
	mCBuffer.BindPS(0);

	mSampler.BindPS(0);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTerrain.Render();
}

//----------------------------------------------------------------------------------------------------

void Terrain::DebugRender()
{
	for (auto& sector : mTerrainSectors)
	{
		SimpleDraw::AddAABB(sector.aabb, Color::Blue());
	}
}