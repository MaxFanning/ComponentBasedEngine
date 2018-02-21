//====================================================================================================
// Filename:	ConstantBuffer.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

ConstantBuffer::ConstantBuffer()
	: mpBuffer(nullptr)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mpBuffer == nullptr, "[ConstantBuffer] Buffer not released!");
}

//----------------------------------------------------------------------------------------------------

void ConstantBuffer::Initialize(u32 bufferSize, const void* initData)
{
	GraphicsSystem* gs = GraphicsSystem::Get();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof desc);
	desc.ByteWidth = bufferSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA subresourceData;
	D3D11_SUBRESOURCE_DATA* subresourceDataPtr = nullptr;
	if (initData != nullptr)
	{
		ZeroMemory(&subresourceData, sizeof(subresourceData));
		subresourceData.pSysMem = initData;
		subresourceDataPtr = &subresourceData;
	}

	gs->GetDevice()->CreateBuffer(&desc, subresourceDataPtr, &mpBuffer);
}

//----------------------------------------------------------------------------------------------------

void ConstantBuffer::Terminate()
{
	SafeRelease(mpBuffer);
}

//----------------------------------------------------------------------------------------------------

void ConstantBuffer::Set(const void* data)
{
	GraphicsSystem::Get()->GetContext()->UpdateSubresource(mpBuffer, 0, nullptr, data, 0, 0);
}

//----------------------------------------------------------------------------------------------------

void ConstantBuffer::BindVS(u32 slot)
{
	GraphicsSystem::Get()->GetContext()->VSSetConstantBuffers(slot, 1, &mpBuffer);
}

//----------------------------------------------------------------------------------------------------

void ConstantBuffer::BindGS(u32 slot)
{
	GraphicsSystem::Get()->GetContext()->GSSetConstantBuffers(slot, 1, &mpBuffer);
}

//----------------------------------------------------------------------------------------------------

void ConstantBuffer::BindPS(u32 slot)
{
	GraphicsSystem::Get()->GetContext()->PSSetConstantBuffers(slot, 1, &mpBuffer);
}

//----------------------------------------------------------------------------------------------------

void ConstantBuffer::BindCS(u32 slot)
{
	GraphicsSystem::Get()->GetContext()->CSSetConstantBuffers(slot, 1, &mpBuffer);
}