#pragma once
#include <windows.h>
struct CMElement
{
	enum STATE { ALLOCED = -1 };
	__forceinline CMElement(int objectSize, int objectNum, int seqNum)
		:objectSize_{ objectSize }, objectNum_{ objectNum }, seqNum_{ seqNum }, pNext_{ nullptr }
	{
		int bufSize = (objectSize + sizeof(ULONGLONG)) * objectNum;
		pBuf_ = new char[bufSize];
		indexTable_ = new int[objectNum];
		top_ = -1;
		for (int i = 0; i < objectNum; ++i)
		{
#pragma warning (disable : 6011)
			* (ULONGLONG*)(pBuf_ + (objectSize_ + sizeof(ULONGLONG)) * i) = seqNum_;
			indexTable_[i] = i;
#pragma warning (default : 6011)
		}
	}
	__forceinline ~CMElement()
	{
		delete[] pBuf_;
		delete[] indexTable_;
	}

	__forceinline char* Alloc()
	{
		if (top_ == objectNum_ - 1)
		{
			return nullptr;
		}
		int index = indexTable_[++top_];
		char* retAddr = pBuf_ + (objectSize_ + sizeof(ULONGLONG)) * index;
		indexTable_[top_] = ALLOCED;
		return retAddr;
	}

	__forceinline void Free(char* freeAddr)
	{
		// 여기들어오는 freeAddr은 몇번째 리스트인지 써잇는 시퀀스번호의 시작주소
#ifdef _DEBUG
		if (top_ == -1)
		{
			__debugbreak();
		}
#endif
		// indexTable에서 top이 가리키는 위치에 현재 반환하는 메모리의 인덱스를 기록함
		indexTable_[top_--] = (int)((freeAddr - pBuf_) / (objectSize_ + sizeof(ULONGLONG)));
	}
	char* pBuf_;
	int objectSize_;
	int objectNum_;
	int seqNum_;
	int* indexTable_;
	int top_;
	CMElement* pNext_;
};

