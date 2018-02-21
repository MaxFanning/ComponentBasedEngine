#ifndef ARENA_ALLOCATOR_HEADER_H
#define ARENA_ALLOCATOR_HEADER_H

class ArenaAllocator
{
public:
	ArenaAllocator(u32 capacity);
	~ArenaAllocator();

	void* Allocate(u32 size);
	void Rewind();

private:
	u8* mData;
	s32 mNextAlloc;
	u32 mCapacity;
};

#endif