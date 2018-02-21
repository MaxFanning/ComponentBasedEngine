#include "Precompiled.h"
#include "BlockAllocator.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

namespace // Anonymous namespace, encloses function to only be used in this file
{
	s32& GetBlockAsInt(u8* data, u32 index, u32 blockSize)
	{
		return *(s32*)(data + (index * blockSize));
	}
}

s32& BlockAllocator::GetBlockAsInt(u32 index) // Casts the block to int to be retrieved using the index
{
	return *reinterpret_cast<s32*>(mData + (index * mBlockSize));
}

void BlockAllocator::EnsurePtrValid(void* ptr)
{
	// Assert that the ptr is valid
	ASSERT(ptr >= mData && ptr < mData + (mBlockSize * mCapacity), "[BlockAllocator] Invalid pointer.");
	ASSERT(((u8*)ptr - mData) % mBlockSize == 0, "[BlockAllocator] Invalid pointer");
}

BlockAllocator::BlockAllocator(u32 blockSize, u32 capacity)
	: mBlockSize((std::max)(sizeof(s32), blockSize)) // Make sure block size is at least the size of a s32
	, mCapacity(capacity)
	, mData(nullptr)
	, mNextSlot(0)
{
	mData = (u8*)malloc(mBlockSize * capacity); // Allocating to mData

	// Link free blocks to next one
	for (s32 i = 0; i + 1 < (s32)capacity; i++)
	{
		GetBlockAsInt(i) = i + 1;
	}

	// Sets last block to -1 to be able to do checks later
	GetBlockAsInt(capacity - 1) = -1;
}

BlockAllocator::~BlockAllocator()
{
	free(mData);
}

void* BlockAllocator::Allocate()
{
	if (mNextSlot == -1) // Returns nullptr if it cannot allocate
	{
		return nullptr;
	}

	void* freeSlot = mData + (mNextSlot * mBlockSize); // Creates temp to hold allocated block
	mNextSlot = *(s32*)freeSlot;
	return freeSlot;
}

void BlockAllocator::Free(void* ptr)
{
	if (ptr == nullptr) // If not an actual block
	{
		return;
	}

	EnsurePtrValid(ptr);

	s32 slotIndex = (s32)(((u8*)ptr - mData) / mBlockSize); // Temp to hold block
	*(s32*)ptr = mNextSlot;
	mNextSlot = slotIndex; // Sets this block to be the next block to be allocated from
}