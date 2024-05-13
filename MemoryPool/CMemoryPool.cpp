#include <malloc.h>
#include "CMemoryPool.h"
#include "MemoryPool.h"
#define ALLOCED -1

CMemoryPool::CMemoryPool(int objectSize, int maxBufferSize)
{
	objectSize_ = objectSize;
	int num = maxBufferSize / objectSize;
	pool_ = malloc(maxBufferSize);
	if (pool_ == nullptr)	__debugbreak();
	indexTable = (int*)malloc(sizeof(int) * num);
	top = -1;
	for (int i = 0; i < num; ++i)
	{
#pragma warning (disable : 6011)
		indexTable[i] = i;
#pragma warning (default : 6011)
	}
}

void* CMemoryPool::Alloc()
{
	void* retAddr;
	int index;
	++top;
	index = indexTable[top];
	retAddr = (char*)pool_ + objectSize_ * index;
	indexTable[top] = ALLOCED;
	return retAddr;
}

void CMemoryPool::Free(void* freeAddr)
{
	// indexTable에서 top이 가리키는 위치에 현재 반환하는 메모리의 인덱스를 기록함
	indexTable[top--] = (int)(((char*)freeAddr - (char*)pool_) / objectSize_);
}

CMemoryPool::~CMemoryPool()
{
	free(pool_);
	free(indexTable);
}

