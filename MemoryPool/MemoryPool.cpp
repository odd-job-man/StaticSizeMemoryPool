#include "CMemoryPool.h"
#define MEMORYPOOLAPI extern "C" __declspec(dllexport)
#include "MemoryPool.h"

MEMORYPOOLAPI MEMORYPOOL CreateMemoryPool(DWORD objectSize, DWORD maxBufferSize)
{
    return new CMemoryPool(objectSize, maxBufferSize);
}

MEMORYPOOLAPI PVOID AllocMemoryFromPool(MEMORYPOOL MP)
{
    return ((CMemoryPool*)(MP))->Alloc();
}

MEMORYPOOLAPI VOID RetMemoryToPool(MEMORYPOOL MP, PVOID freeAddr)
{
    return ((CMemoryPool*)(MP))->Free(freeAddr);
}

MEMORYPOOLAPI VOID ReleaseMemoryPool(MEMORYPOOL MP)
{
    delete ((CMemoryPool*)MP);
}








