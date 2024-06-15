#include "CMemoryPool.h"
#define MEMORYPOOLAPI extern "C" __declspec(dllexport)
#include "MemoryPool.h"

MEMORYPOOLAPI MEMORYPOOL CreateMemoryPool(int objectSize, int objectNum)
{
    return new CMemoryPool(objectSize, objectNum);
}

MEMORYPOOLAPI PVOID AllocMemoryFromPool(MEMORYPOOL MP)
{
    return ((CMemoryPool*)(MP))->Alloc();
}

MEMORYPOOLAPI VOID RetMemoryToPool(MEMORYPOOL MP, PVOID freeAddr)
{
    return ((CMemoryPool*)(MP))->Free((char*)freeAddr);
}

MEMORYPOOLAPI VOID ReleaseMemoryPool(MEMORYPOOL MP)
{
    delete ((CMemoryPool*)MP);
}








