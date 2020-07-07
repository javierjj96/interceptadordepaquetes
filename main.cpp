#include "HookFunctions.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <iostream>
#include <conio.h>

#include <d3d9.h>
#include <dwmapi.h>
#include <TlHelp32.h>
#include <string>


int todoelsocket;
int repetir = 0;
SOCKET mysocket;
int flags = 0;
const char* buypots;
int lenbuypots;
typedef SHORT(WINAPI* tGetAsyncKeyState)(__in  int vKey);
int contador = 0;
bool potmana = false;
void presionar(DWORD key) {
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wVk = 0; //We're doing scan codes instead
	ip.ki.dwExtraInfo = 0;

	//This let's you do a hardware scan instead of a virtual keypress
	ip.ki.dwFlags = KEYEVENTF_SCANCODE;
	ip.ki.wScan = key; // 0x1C;  //Set a unicode character to use (A)

						 //Send the press
	SendInput(1, &ip, sizeof(INPUT));
	Sleep(10);
	//Prepare a keyup event
	ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));

}
void __declspec(naked) g_pSysCall()
{
	__asm
	{
		mov eax, 0x30000
		mov edx, 0x76687810
		call edx
		retn 4
	}
}

bool HookKeystate()
{
	if (*(DWORD*)((DWORD)g_pSysCall + 1) != 0x30000) return true; // redunant check

	DWORD pFunc = (DWORD)GetProcAddress(GetModuleHandle("win32u.dll"), "NtUserGetAsyncKeyState");
	if (!pFunc) return false;

	DWORD FunctionIndex = *(DWORD*)(pFunc + 1);
	DWORD CallIndex = *(DWORD*)(pFunc + 6);
	if (!FunctionIndex || !CallIndex) return false;

	*(DWORD*)((DWORD)g_pSysCall + 1) = FunctionIndex;
	*(DWORD*)((DWORD)g_pSysCall + 6) = CallIndex;

	return true;
}


SHORT __stdcall oGetAsyncKeyState(int vKey)
{
	tGetAsyncKeyState lGetAsyncKeyState = (tGetAsyncKeyState)g_pSysCall;
	return lGetAsyncKeyState(vKey);
}

std::string hexStr(unsigned char* data, int len)
{
	std::stringstream ss;
	ss << std::hex;
	for (int i = 0; i < len; ++i)
		ss << std::setw(2) << std::setfill('0') << (int)data[i];
	return ss.str();

}
int __stdcall Hook_Recv(SOCKET s, char* buf, int len, int flags)
{
	
	std::string packet = buf;
	const char* str = buf;
	std::string hex = hexStr((unsigned char*)packet.c_str(), packet.length());
	//std::cout << std::hex << packet;
	//printf("Recv Largo: %d\n", hex.c_str(), len);
	//printf("%s Largo: %d\n", hex.c_str(), len);

	return Real_Recv(s, buf, len, flags);
}


int __stdcall Hook_Send(SOCKET s, const char* buf, int len, int flags)
{
	mysocket = s;
	/*
	std::string packet = buf;
	printf("En char %c\n", *buf);*/
	//std::string packet = buf;
	//const char* str = buf;
	//std::string hex = hexStr((unsigned char*)packet.c_str(), packet.length());
	//
	//std::cout << std::hex << packet;
	//printf("%s Largo: %d\n", hex.c_str(), len);

	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (int i = 0; i < 32; ++i)
	{
		ss << std::setw(2) << static_cast<unsigned>(buf[i]);
	}
	std::string mystr = ss.str();

	if (len!=7 &&  len != 11 )
	{
		printf("%s Largo: %d Flags: %d SOCKET %d \n", mystr.c_str(), len, flags, s);
	}
	

	//Real_Send(s, buf2, len+len, flags);

	//	printf("Send\n");
	//if (len == 11 ) {
	//	printf("Encontrado \n");
	//	buypots = buf;
	//	lenbuypots = len;
	//	std::stringstream xd;



	//	//Real_Send(s, buf2, len + len, flags);

	//}
	/*if (mystr.find("ffffffaa5530372cffffffff55ffffffaa") != std::string::npos && len == 9) {
		printf("Comprando potas \n");
		

		return Real_Send(s, buypots, lenbuypots, flags);

		

	}*/
	if (mystr.find("ffffffaa55") != std::string::npos && len == 16) {
		printf("Encontrado potas \n");
		buypots = buf;
		lenbuypots = len;
		std::stringstream xd;

		todoelsocket = Real_Send(s, buf, len, flags);



	}



	//printf("%02hhx",mensaje);
	if (repetir)
	{
	

		//printf("Repetir \n");
		if (mystr.find("ffffffaa55c08") != std::string::npos && len == 18) {
		
		
			for (size_t i = 0; i < repetir; i++)
			{
				Real_Send(s, buf, len, flags);
				
			}



		}
		if (mystr.find("ffffffaa551408") != std::string::npos && len == 26 || mystr.find("ffffffaa55140008") != std::string::npos && len == 26 &&  mystr.find("003") != std::string::npos || mystr.find("ffffffaa55140008") != std::string::npos && len == 26 && mystr.find("022") != std::string::npos) {



			//std::string mystr2 = ss.str() + ss.str();
			//const char* buf2 = mystr2.c_str();
			////return Real_Send(s, buf2, len + len, flags);
			////return Real_Send(s, buf2, len, flags);
			contador++;
		//	printf("Contador %d \n", contador);
			if (mystr.find("ffffffaa551408") != std::string::npos &&  contador == 4)
			{
				
				potmana = true;
			
				contador = 0;
			}
			
			if (repetir==1)
			{
				Real_Send(s, buf, len, flags);
			}
			if (repetir == 2)
			{
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
			}
			if (repetir == 3)
			{
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				
				
			}
			if (repetir == 4)
			{
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
		
			}
			if (repetir == 5)
			{
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
		
			}
			if (repetir == 6)
			{
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
				Real_Send(s, buf, len, flags);
		
			}
			
				

			
			
		

			
		
				

		}

		
		

		//Real_Send(s, buf2, len + len, flags);
		//Sleep(1000);
	}


	return Real_Send(s, buf, len, flags);
}





void xD() {

	//DWORD pid;
	//HANDLE hndl;

	//int value = { 0 };

	//HWND window = FindWindow(NULL, "devperfTest");

	//GetWindowThreadProcessId(window, &pid);
	//hndl = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

	//uintptr_t modBase = GetModuleBaseAddress(pid, "devperfTest.exe");
	//while (true)
	//{
	//	if (ReadProcessMemory(hndl, (LPVOID)(modBase + 0x3374), &value, sizeof(value), NULL))
	//	{
	//		printf("Repetir %d \n", value);
	//		Sleep(100);
	//	}
	//	

	//}

	int hp = 1;
	int hp2 = 2;
	printf("Repetir %d\n", repetir);
	HookKeystate();
	while (true)
	{
	
		Sleep(50);
	
	
		PutJMP(Get_Send(), Hook_Send);
	
	/*	if (ReadProcessMemory(GetCurrentProcess(), (LPCVOID)0x02a6a488, &hp, sizeof(int), NULL))
		{
			ReadProcessMemory(GetCurrentProcess(), (LPCVOID)0x07f36154, &hp2, sizeof(int), NULL);
			printf("HP = %d \n", hp);
			printf("HP2 = %d \n", hp2);
		}*/
		
	
		
		if (potmana)
		{
			presionar(0x03);
			potmana = false;
		}
		if (oGetAsyncKeyState(VK_XBUTTON1)) {

			
			repetir = repetir+1;
			system("cls");
			printf("Repetir %d\n", repetir+1);
		}
		if (oGetAsyncKeyState(VK_XBUTTON2)) {
			repetir = 0;
			system("cls");
			printf("Repetir %d\n", repetir+1);
		}
		if (oGetAsyncKeyState(VK_F6)) {
			Sleep(100);
			
			Real_Send(mysocket, buypots, lenbuypots, flags);
			std::stringstream xd;			
			for (int i = 0; i < lenbuypots; ++i)
				xd << std::hex << (int)buypots[i];
			std::string mystr2 = xd.str();
			printf("%s Largo: %d Flags: %d \n", mystr2.c_str(), lenbuypots, flags);
			
		}

		//PutJMP(Get_Recv(), Hook_Recv);
	}
	
	
		
	
}



void teclas()
{
	
}

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
) {
	
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		//CreateDirectoryA("Recv", NULL);
	//	CreateDirectoryA("Send", NULL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)xD, 0, 0, 0);
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)teclas, 0, 0, 0);
		
		

		break;
	
		break;
	}
	return 1;
}

