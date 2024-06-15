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
		// ��������� freeAddr�� ���° ����Ʈ���� ���մ� ��������ȣ�� �����ּ�
#ifdef _DEBUG
		if (top_ == -1)
		{
			__debugbreak();
		}
#endif
		// indexTable���� top�� ����Ű�� ��ġ�� ���� ��ȯ�ϴ� �޸��� �ε����� �����
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

