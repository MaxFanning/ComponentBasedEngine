#ifndef INCLUDED_ENGINE_BLOCKALLOCATOR_H
#define INCLUDED_ENGINE_BLOCKALLOCATOR_H

class BlockAllocator
{
public:
	BlockAllocator(u32 blockSize, u32 capacity);
	~BlockAllocator();

	void* Allocate();
	void Free(void* ptr);

protected:
	s32& GetBlockAsInt(u32 index);
	void EnsurePtrValid(void* ptr);

	u8* mData; // Allocated space
	u32 mBlockSize; // Size of a block
	u32 mCapacity; // How many blocks
	s32 mNextSlot; // Next free block

	// std::vector<u8*> mFreeSlots;
};

#endif