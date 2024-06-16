#pragma once
#include <windows.h>
struct CMElement
{
	enum STATE { ALLOCED = -1 };
	__forceinline CMElement(int objectSize, int elementNum, int seqNum)
		:elementNum_{ elementNum }, seqNum_{ seqNum }, pNext_{ nullptr }
	{
		int quotient = objectSize / sizeof(ULONGLONG);
		if (objectSize % sizeof(ULONGLONG) > 0)
		{
			++quotient;
		}
		int padding = sizeof(ULONGLONG) * quotient - objectSize;
		elementSize_ = objectSize + sizeof(ULONGLONG) + padding;
		pBuf_ = new char[elementSize_ * elementNum];
		indexTable_ = new int[elementNum];
		top_ = -1;
		for (int i = 0; i < elementNum; ++i)
		{
#pragma warning (disable : 6011)
			* (ULONGLONG*)(pBuf_ + elementSize_ * i) = seqNum_;
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
		if (top_ == elementNum_ - 1)
		{
			return nullptr;
		}
		int index = indexTable_[++top_];
		char* retAddr = pBuf_ + elementSize_ * index;
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
		indexTable_[top_--] = (int)((freeAddr - pBuf_) / elementSize_);
	}

	__forceinline int ReportLeak()
	{
		int i;
		for (i = 0; i < elementNum_ && indexTable_[i] == ALLOCED; ++i);
		return i;
	}

	char* pBuf_;
	int elementNum_;
	int elementSize_;
	int seqNum_;
	int* indexTable_;
	int top_;
	CMElement* pNext_;
};

