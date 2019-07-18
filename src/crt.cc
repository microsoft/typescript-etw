// The absolute minimum to compile without any CRT libraries present

#define WINVER 0x0601 // Support Win7 or later
#include <Windows.h>

void * __cdecl operator new(size_t s)
{
	return HeapAlloc(GetProcessHeap(), 0, s);
}

void __cdecl operator delete(void * p)
{
	HeapFree(GetProcessHeap(), 0, p);
}

void __cdecl operator delete(void* const block, size_t const) noexcept
{
	operator delete(block);
}
