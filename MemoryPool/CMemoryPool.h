#pragma once
class CMemoryPool
{
public:
	CMemoryPool(int objectSize, int maxBufferSize);
	void* Alloc();
	void Free(void* freeAddr);
	int objectSize_;
	void* pool_;
	int* indexTable;
	int top;
	~CMemoryPool();
};
