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
	// indexTable���� top�� ����Ű�� ��ġ�� ���� ��ȯ�ϴ� �޸��� �ε����� �����
	indexTable[top--] = (int)(((char*)freeAddr - (char*)pool_) / objectSize_);
}

CMemoryPool::~CMemoryPool()
{
	free(pool_);
	free(indexTable);
}

