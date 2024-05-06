#pragma once
class MemoryPool
{
public:
	MemoryPool(size_t objectSize, size_t maxBufferSize);
	void* Alloc();
	void Free(void* freeAddr);
	size_t objectSize_;
	void* pool_;
	int* indexTable;
	int top;
	~MemoryPool();
};

