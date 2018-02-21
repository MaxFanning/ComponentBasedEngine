#ifndef INCLUDED_ENGINE_TYPEDALLOCATOR_H
#define INCLUDED_ENGINE_TYPEDALLOCATOR_H
#include "BlockAllocator.h"

template <typename T>
class TypedAllocator : private BlockAllocator
{
public:
	TypedAllocator(u32 capacity);
	~TypedAllocator();

	template<typename ...Args>
	T* New(Args&&... args); // Do placement new
	void Delete(T* ptr);
};

template <typename T>
TypedAllocator<T>::TypedAllocator(u32 capacity)
	: BlockAllocator(sizeof(T), capacity)
{
}

template <typename T>
TypedAllocator<T>::~TypedAllocator()
{
}

template <typename T>
template<typename... Args>
T* TypedAllocator<T>::New(Args&&... args)
{
	void* instance = Allocate();

	if (instance)
	{
		new(instance) T(std::forward<Args>(args)...);
	}

	return static_cast<T*>(instance);
}

template <typename T>
void TypedAllocator<T>::Delete(T* ptr)
{
	if (ptr == nullptr)
	{
		return;
	}
	
	EnsurePtrValid(ptr);

	ptr->~T();
	Free(ptr);
	ptr = nullptr;
}

#endif