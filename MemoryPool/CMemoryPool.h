#pragma once
#include "CMElement.h"
class CMemoryPool
{
public:
	__forceinline CMemoryPool(int objectSize, int objectNum)
		:objectSize_{ objectSize }, poolNum_{ 1 }, objectNum_{ objectNum }
	{
		pPool_ = new CMElement(objectSize, objectNum, poolNum_ - 1);
	}
	__forceinline void* Alloc()
	{
		CMElement* pPool = pPool_;
		CMElement* pPrev;
		char* retAddr;
		while (pPool)
		{
			retAddr = pPool->Alloc();
			if (!retAddr)
			{
				pPrev = pPool;
				pPool = pPool->pNext_;
			}
			else
			{
				return retAddr + sizeof(ULONGLONG);
			}
		}
		// ������� �Ӵٴ°� ���� �����ϴ� Ǯ���� �� �����ٴ� ���̴�.
		pPool = new CMElement{ objectSize_,objectNum_,poolNum_++ };
		pPrev->pNext_ = pPool;
		retAddr = pPool->Alloc() + sizeof(ULONGLONG);
		return retAddr;
	}
	__forceinline void Free(char* freeAddr)
	{
		// ���� ����ִ� freeAddr�� seqNum������ �����Ͱ� �����ϴ� �ּ�
		freeAddr -= sizeof(ULONGLONG);
		ULONGLONG poolSeqNum = *(ULONGLONG*)(freeAddr);
		CMElement* pPool = pPool_;
		for (ULONGLONG i = 0; i < poolSeqNum; ++i)
		{
			pPool = pPool->pNext_;
		}
		pPool->Free(freeAddr);
	}
	__forceinline ~CMemoryPool()
	{
		CMElement* pPool = pPool_;
		CMElement* pPoolTemp;
		for (int i = 0; i < poolNum_; ++i)
		{
			pPoolTemp = pPool;
			pPool = pPool->pNext_;
			delete pPoolTemp;
		}
	}

	int objectSize_;
	int objectNum_;
	int poolNum_;
	CMElement* pPool_;
};

