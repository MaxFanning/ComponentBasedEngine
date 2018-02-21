#ifndef INCLUDED_ENGINE_SAMPLER_H
#define INCLUDED_ENGINE_SAMPLER_H

//====================================================================================================
// Filename:	Sampler.h
// Created by:	Peter Chan
// Description:	Class for a texture sampler.
//====================================================================================================

//====================================================================================================
// Forward Declarations
//====================================================================================================

class GraphicsSystem;

//====================================================================================================
// Class Declarations
//====================================================================================================

class Sampler
{
public:
	enum Filter
	{
		Point,
		Linear,
		Anisotropic
	};

	enum AddressMode
	{
		Border,
		Clamp,
		Mirror,
		Wrap,
	};

	Sampler();
	~Sampler();
	
	void Initialize(Filter filter, AddressMode addressMode);
	void Terminate();
	
	void BindVS(u32 index);
	void BindGS(u32 index);
	void BindPS(u32 index);

private:
	ID3D11SamplerState* mpSampler;
};

#endif // #ifndef INCLUDED_ENGINE_SAMPLER_H
