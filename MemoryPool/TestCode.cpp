//#include "CMemoryPool.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#pragma comment(lib,"MemoryPoolD.lib")
#else
#pragma comment(lib,"MemoryPool.lib")
#endif
#include <new>
#include "MemoryPool.h"

constexpr int NumPerBuffer = 100;

struct Test
{
	char a;
	int b;
	char c;
	char d;
	Test()
	{
		a = 1;
		b = 2;
		c = 3;
		d = 4;
	}
};

int main()
{
	MEMORYPOOL MP = CreateMemoryPool(sizeof(Test), NumPerBuffer);
	Test* arr[NumPerBuffer * 3];
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
	{
		arr[i] = (Test*)AllocMemoryFromPool(MP);
		new(arr[i])Test{};
	}

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
	{
		RetMemoryToPool(MP, arr[i]);
	}


	//arr[0] = (Test*)AllocMemoryFromPool(MP);
	//arr[199] = (Test*)AllocMemoryFromPool(MP);

	int leakNum = ReportLeak(MP);
	if (leakNum != 0)
	{
		__debugbreak();
	}

	ReleaseMemoryPool(MP);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
}
