#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <io.h>
#include "shellcode.hpp"
#include "dll_shell.h"
#include "VMP/VMProtectSDK.h"

#pragma warning(disable : 4996)

void get_rand_str(char s[], int num){
	//定义随机生成字符串表
	char str[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int i, lstr;
	char ss[2] = { 0 };
	lstr = strlen(str);//计算字符串长度
	srand((unsigned int)time((time_t*)NULL));//使用系统时间来初始化随机数发生器
	for (i = 1; i <= num; i++) {//按指定大小返回相应的字符串
		sprintf(ss, "%c", str[(rand() % lstr)]);//rand()%lstr 可随机返回0-71之间的整数, str[0-71]可随机得到其中的字符
		strcat(s, ss);//将随机生成的字符串连接到指定数组后面
	}
}

DWORD GetProcessIdByName(const wchar_t* ProcessName) {
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		return 0;
	}
	PROCESSENTRY32 info = { 0 };
	info.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snap, &info)) {
		CloseHandle(snap);
		return 0;
	}
	do
	{
		if (!wcscmp(ProcessName, info.szExeFile)) {
			CloseHandle(snap);
			return info.th32ProcessID;
		}
	} while (Process32Next(snap, &info));
	CloseHandle(snap);
	return 0;
}


PCHAR ReadFileToBit(const char* Path, PDWORD lplength) {
	HANDLE hfile = CreateFileA(Path, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (INVALID_HANDLE_VALUE == hfile) {
		return nullptr;
	}
	DWORD fsize = GetFileSize(hfile, NULL);
	PCHAR dll_data = (PCHAR)malloc((size_t)fsize + 1);
	BOOL result = ReadFile(hfile, dll_data, fsize, NULL, NULL);
	
	if (!result) {
		free(dll_data);
	}
	else {
		DeleteFileA(Path);
	}
	CloseHandle(hfile);
	*lplength = fsize;
	return !result ? nullptr : dll_data;
}
bool Injecting(DWORD ProcessId, const unsigned char* DllByte, SIZE_T Dll_byte_size) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessId);
	if (!hProcess) {
		printf("OpenProcess failed!\n");
		return false;
	}

#if _WIN64
	PUCHAR shellcodes = shellcode_64;
	DWORD shellSize = shellcode_64_size;
#endif // _WIN64
	PUCHAR shellcodes = shellcode_32;
	DWORD shellSize = shellcode_32_size;

	LPVOID shellcode_addr = VirtualAllocEx(hProcess, NULL, shellSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	LPVOID dll_addr = VirtualAllocEx(hProcess, NULL, Dll_byte_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	HANDLE hthread = 0;
	if (shellcode_addr && dll_addr) {
		if (!WriteProcessMemory(hProcess, shellcode_addr, shellcodes, shellSize, NULL) ||
			!WriteProcessMemory(hProcess, dll_addr, DllByte, Dll_byte_size, NULL)) {
			printf("WriteProcessMemory failed!\n");
			goto error;
		}
	}
	else {
		printf("VirtualAllocEx failed!\n");
		CloseHandle(hProcess);
		return false;
	}
	hthread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)shellcode_addr, dll_addr, 0, NULL);
	if (hthread) {
		WaitForSingleObject(hthread, INFINITE);
		CloseHandle(hthread);
		goto success;
	}
error:
	VirtualFreeEx(hProcess, dll_addr, 0, MEM_RELEASE);
success:
	VirtualFreeEx(hProcess, shellcode_addr, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	return true;
}
bool Injecting(DWORD ProcessId, PCHAR DllPath) {
	DWORD fsize = 0;
	PCHAR dll_bit = ReadFileToBit(DllPath, &fsize);
	if (!dll_bit || !fsize) {
		printf("ReadFile failed!\n");
		return false;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessId);
	if (!hProcess) {
		printf("OpenProcess failed!\n");
		free(dll_bit);
		return false;
	}
#if _WIN64
	PUCHAR shellcodes = shellcode_64;
	DWORD shellSize = shellcode_64_size;
#endif // _WIN64
	PUCHAR shellcodes = shellcode_32;
	DWORD shellSize = shellcode_32_size;

	LPVOID shellcode_addr = VirtualAllocEx(hProcess, NULL, shellSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	LPVOID dll_addr = VirtualAllocEx(hProcess, NULL, fsize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	HANDLE hthread = 0;
	if (shellcode_addr && dll_addr) {
		if (!WriteProcessMemory(hProcess, shellcode_addr, shellcodes, shellSize, NULL) ||
			!WriteProcessMemory(hProcess, dll_addr, dll_bit, fsize, NULL)) {
			printf("WriteProcessMemory failed!\n");
			goto error;
		}
	}
	else {
		printf("VirtualAllocEx failed!\n");
		CloseHandle(hProcess);
		free(dll_bit);
		return false;
	}
	hthread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)shellcode_addr, dll_addr, 0, NULL);
	if (hthread) {
		WaitForSingleObject(hthread, INFINITE);
		CloseHandle(hthread);
		goto success;
	}
	error:
	VirtualFreeEx(hProcess, dll_addr, 0, MEM_RELEASE);
	success:
	VirtualFreeEx(hProcess, shellcode_addr, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	free(dll_bit);
	return true;
}

void error() {
	Sleep(2000);
	exit(0);
}

typedef struct _APC_MEM_INJECT {
	DWORD pid;
	DWORD dll_size;
	PCHAR dll_byte;
}APC_MEM_INJECT, * PAPC_MEM_INJECT;

int main() {
	//VMProtectBegin("main");
	DWORD pid = GetProcessIdByName(L"csgo.exe");
	if (!pid) {
		printf("GetProcessIdByName error\n");
		error();
	}
	
	CHAR path[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, path);
	strcat_s(path, "\\CSG0_Dx.dll");
	if (_access(path, 0) == -1) {
		printf("dll文件不存在\n");
		error();
	}
	bool result = false;
	//result = Injecting(pid, dll_char, sizeof(dll_char));
	result = Injecting(pid, path);
	if (!result) {
		printf("memory inject dll failed\n");
	}
	else {
		printf("success\n");
	}
	error();
	//VMProtectEnd();
}