#ifndef INCLUDED_ENGINE_CONSTANTBUFFER_H
#define INCLUDED_ENGINE_CONSTANTBUFFER_H

//====================================================================================================
// Filename:	ConstantBuffer.h
// Created by:	Peter Chan
// Description:	Class for a constant buffer.
//====================================================================================================

//====================================================================================================
// Forward Declarations
//====================================================================================================

class GraphicsSystem;

//====================================================================================================
// Class Declarations
//====================================================================================================

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Initialize(u32 bufferSize, const void* initData = nullptr);
	void Terminate();

	void Set(const void* data);
	
	void BindVS(u32 slot = 0);
	void BindGS(u32 slot = 0);
	void BindPS(u32 slot = 0);
	void BindCS(u32 slot = 0);

private:
	ID3D11Buffer* mpBuffer;
};

//----------------------------------------------------------------------------------------------------

template <typename T>
class TypedConstantBuffer : public ConstantBuffer
{
public:
	void Initialize()
	{
		ConstantBuffer::Initialize(GetPaddedSize());
	}

	void Initialize(const T& data)
	{
		ConstantBuffer::Initialize(GetPaddedSize(), &data);
	}

	void Set(const T& data)
	{
		ConstantBuffer::Set(&data);
	}

private:
	u32 GetPaddedSize() const
	{
		const u32 typeSize = sizeof(T);
		const u32 bufferSize = (typeSize % 16) ? ((typeSize / 16) + 1) * 16 : typeSize;
		return bufferSize;
	}
};

#endif // #ifndef INCLUDED_ENGINE_CONSTANTBUFFER_H