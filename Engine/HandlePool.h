#ifndef INCLUDED_ENGINE_HANDLEPOOL_H
#define INCLUDED_ENGINE_HANDLEPOOL_H

#include "Common.h"

template <typename DataType>
class HandlePool;

template <typename DataType>
class Handle
{
public:
	Handle();

	bool IsValid() const;
	void Invalidate();

	DataType* Get() const;

	bool operator==(Handle rhs) const { return mIndex == rhs.mIndex && mGeneration == rhs.mGeneration; }
	bool operator!=(Handle rhs) const { return !(*this == rhs); }

private:
	friend class HandlePool<DataType>;
	static HandlePool<DataType>* sPool;

	u32 mIndex : 24; //bit packing
	u32 mGeneration : 8; //bit packing
};

template <typename DataType>
class HandlePool
{
public:
	typedef Handle<DataType> HandleType;

	HandlePool(u32 capacity);
	~HandlePool(); //clear what is done in constructor

	HandleType Register(DataType* instance);
	void Unregister(HandleType handle);

	DataType* Get(HandleType handle);
	bool IsValid(HandleType handle) const;

private:
	struct Entry
	{
		DataType* instance = nullptr;
		u32 generation = 0;
	};

	std::vector<Entry> mEntries; //use resize
	std::vector <u32> mFreeSlots; //from 1 to max
};

template <typename DataType>
HandlePool<DataType>* Handle<DataType>::sPool = nullptr;

template <typename DataType>
Handle<DataType>::Handle()
	: mIndex(0)
	, mGeneration(0)
{
}

template <typename DataType>
bool Handle<DataType>::IsValid() const
{
	return sPool && sPool->IsValid(*this);
}

template <typename DataType>
void Handle<DataType>::Invalidate()
{
	&this = Handle();
}

template <typename DataType>
DataType* Handle<DataType>::Get() const
{
	return sPool ? sPool->Get(*this) : nullptr;
}

//---------------------------------------------------------------------

template <typename DataType>
HandlePool<DataType>::HandlePool(u32 capacity)
{
	ASSERT(capacity > 0, "[HandlePool] Invalid Capacity!");

	mEntries.resize(capacity + 1);
	mFreeSlots.reserve(capacity);
	for (size_t i = capacity; i > 0; --i)
	{
		mFreeSlots.push_back(i);
	}

	ASSERT(HandleType::sPool == nullptr, "[HandlePool] Cannot have more than one pool at a time!");
	HandleType::sPool = this;
}

template <typename DataType>
HandlePool<DataType>::~HandlePool()
{
	//check all handles are free
	ASSERT(mFreeSlots.size() == mFreeSlots.capacity(), "[HandlePool] There are still registered slots!");

	ASSERT(HandleType::sPool == this, "[HandlePool] sPool is Not This");
	HandleType::sPool = nullptr;
}

template <typename DataType>
Handle<DataType> HandlePool<DataType>::Register(DataType* instance)
{
	ASSERT(instance != nullptr, "[HandlePool] Invalid Instance!");
	ASSERT(!mFreeSlots.empty(), "[HandlePool] No more free slots available!");

	// draw the next free slot
	u32 slot = mFreeSlots.back();
	mFreeSlots.pop_back();

	// register the isntance
	mEntries[slot].instance = instance;

	//return the handle to this entry
	HandleType handle;
	handle.mIndex = slot;
	handle.mGeneration = mEntries[slot].generation;
	return handle;
}

template <typename DataType>
void HandlePool<DataType>::Unregister(HandleType handle)
{
	if (IsValid(handle))
	{
		// increment the generation to invalidate all existing handles
		mEntries[handle.mIndex].generation++;
		mFreeSlots.push_back(handle.mIndex);
	}
}

template <typename DataType>
DataType* HandlePool<DataType>::Get(HandleType handle)
{
	return IsValid(handle) ? mEntries[handle.mIndex].instance : nullptr;
}

template <typename DataType>
bool HandlePool<DataType>::IsValid(HandleType handle) const
{
	return handle != HandleType() && mEntries[handle.mIndex].generation == handle.mGeneration;
}

#endif // #ifndef INCLUDED_ENGINE_HANDLEPOOL_H
