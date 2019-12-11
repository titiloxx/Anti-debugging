#include <Windows.h>
#include <iostream>

typedef NTSTATUS(__stdcall* pfnNtQueryInformationProcess)(
	_In_      HANDLE           ProcessHandle,
	_In_      UINT             ProcessInformationClass,
	_Out_     PVOID            ProcessInformation,
	_In_      ULONG            ProcessInformationLength,
	_Out_opt_ PULONG           ReturnLength
	);
const UINT ProcessDebugPort = 7;
int main(int argc, char* argv[])
{
	pfnNtQueryInformationProcess NtQueryInformationProcess = NULL;
	NTSTATUS status;
	DWORD isDebuggerPresent = 0;
	HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));

	if (NULL != hNtDll)
	{
		NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
		if (NULL != NtQueryInformationProcess)
		{
			status = NtQueryInformationProcess(
				GetCurrentProcess(),
				ProcessDebugPort,
				&isDebuggerPresent,
				sizeof(DWORD),
				NULL);
			if (status == 0x00000000 && isDebuggerPresent != 0)
			{
				std::cout << "Hay debugger" << std::endl;
				//exit(-1);
			}
			else 
			{
				std::cout << "No hay debugger!" << std::endl;
				std::cout << "Status: " << status<< std::endl;
				std::cout << "isDebuggerPresent: " << isDebuggerPresent<< std::endl;
			}
		}
		while (true)
		{
			Sleep(500);
		}
	}
	return 0;
}