#include <malloc.h>
#include "MemoryPool.h"
#define ALLOCED -1

MemoryPool::MemoryPool(size_t objectSize, size_t maxBufferSize)
{
	objectSize_ = objectSize;
	size_t num = maxBufferSize / objectSize;
	pool_ = malloc(maxBufferSize);
	if (pool_ == nullptr)	__debugbreak();
	indexTable = (int*)malloc(sizeof(int) * num);
	top = -1;
	for (size_t i = 0; i < num; ++i)
	{
#pragma warning (disable : 6011)
		indexTable[i] = i;
#pragma warning (default : 6011)
	}
}

void* MemoryPool::Alloc()
{
	void* retAddr;
	int index;
	++top;
	index = indexTable[top];
	retAddr = (char*)pool_ + objectSize_ * index;
	indexTable[top] = ALLOCED;
	return retAddr;
}

void MemoryPool::Free(void* freeAddr)
{
	int indexToPush;
	indexToPush = ((char*)freeAddr - (char*)pool_) / objectSize_;
	indexTable[top] = indexToPush;
	--top;
}

MemoryPool::~MemoryPool()
{
	free(pool_);
	free(indexTable);
}
