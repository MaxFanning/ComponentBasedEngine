#include "Precompiled.h"
#include "ArenaAllocator.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

ArenaAllocator::ArenaAllocator(u32 capacity)
	: mCapacity(capacity)
	, mNextAlloc(0)
{
	mData = (u8*)malloc(capacity);
}

ArenaAllocator::~ArenaAllocator()
{
	free(mData);
}

void* ArenaAllocator::Allocate(u32 size)
{
	if (size > mCapacity)
	{
		return nullptr;
	}

	if ((mNextAlloc * size) > mCapacity)
	{
		Rewind();
	}

	void* allocatedSpot = mData + (mNextAlloc * size);
	mNextAlloc = *(s32*)allocatedSpot;
	return allocatedSpot;
}

void ArenaAllocator::Rewind()
{
	mNextAlloc = 0;
}