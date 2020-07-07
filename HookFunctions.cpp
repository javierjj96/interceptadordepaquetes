#include "HookFunctions.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>

void* RecvAddr;
void* SendAddr;



__declspec(naked) int __stdcall Real_Recv(
	_In_  SOCKET s,
	_Out_ char   *buf,
	_In_  int    len,
	_In_  int    flags
)
{
	__asm{
		mov edi,edi
		push ebp
		mov ebp,esp
		mov eax, [RecvAddr]
		add eax, 5
		jmp eax
	}
}

__declspec(naked) int __stdcall Real_Send(
	_In_       SOCKET s,
	_In_ const char   *buf,
	_In_       int    len,
	_In_       int    flags
)
{
	__asm{
		mov edi, edi
		push ebp
		mov ebp, esp
		mov eax,[SendAddr]
		add eax,5
		jmp eax
	}
}
const char* string_to_hex(const char* str, char* hex, size_t maxlen)
{
	static const char* const lut = "0123456789ABCDEF";

	if (str == NULL) return NULL;
	if (hex == NULL) return NULL;
	if (maxlen == 0) return NULL;

	size_t len = strlen(str);

	char* p = hex;

	for (size_t i = 0; (i < len) && (i < (maxlen - 1)); ++i)
	{
		const unsigned char c = str[i];
		*p++ = lut[c >> 4];
		*p++ = lut[c & 15];
	}

	*p++ = 0;

	return hex;
}


void * Get_Recv()
{
	RecvAddr = (void*)GetProcAddress(GetModuleHandleA("WS2_32.dll"), "WSARecv");
	return RecvAddr;
}

void * Get_Send()
{
	SendAddr = (void*)GetProcAddress(GetModuleHandleA("wsock32.dll"), "send");
	return SendAddr;
}

void PutJMP(void * WriteTo, void * HookFunction)
{

	DWORD old;
	DWORD bkup;
	VirtualProtect(WriteTo, 0x5, PAGE_EXECUTE_READWRITE, &old);
	memset(WriteTo, 0xE9, 1);
	DWORD RelOffset = (DWORD)HookFunction - (DWORD)WriteTo - 5;
	memcpy((void*)((DWORD)WriteTo + 1), &RelOffset, 4);
	VirtualProtect(WriteTo, 0x5, old, &bkup);
	return;
}




