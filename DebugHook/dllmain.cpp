// dllmain.cpp : Defines the entry point for the DLL application.
#include "windows.h"
#include <iostream>
#include <fstream>
#include <map>
DWORD funcion = 0x77666000;
bool Hook(void* toHook, void* ourFunct, int len)
{
	if (len < 5)
	{
		return false;
	}

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);//El nopeador

	DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);
	return true;
}
void __declspec(naked)ourFunct() {
	__asm {
		jmp[funcion+0x4]
	}
}
DWORD WINAPI MainThread(LPVOID param)
{

	Hook((void*)funcion, ourFunct, 7);
	return 0x0;

	
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		break;
	default:
		break;
	}
	return TRUE;
}

