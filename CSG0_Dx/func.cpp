#include "func.hpp"

void SetConsoleStatus(bool iscreate)
{
	static HWND debug_window = 0;
	if (!iscreate) {
		FreeConsole();
		EnableMenuItem(GetSystemMenu(debug_window, false), SC_CLOSE, MF_ENABLED | MF_BYCOMMAND);
		SendMessage(debug_window, WM_CLOSE, 0, 0);
		return;
	}
	FILE* file = nullptr;
	AllocConsole();
	freopen_s(&file, "CON", "r", stdin);
	freopen_s(&file, "CON", "w", stdout);
	SetConsoleTitleA("debug window");
	debug_window = GetConsoleWindow();
	EnableMenuItem(GetSystemMenu(debug_window, false), SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);
}

DWORD FindPattern(DWORD dwAddress, DWORD dwLength, const char* szPattern, DWORD length)
{
	PCHAR cmpaddr = (PCHAR)dwAddress;
	for (size_t i = 0; i < dwLength; i++){
		ULONG equal = 0;
		for (size_t u = 0; u < length; u++){
			if (*(PCHAR)(cmpaddr + u) == szPattern[u])
				equal++;
			else
				break;
		}
		if (equal == length)
			return (DWORD)cmpaddr;
		cmpaddr++;
	}
	return 0;
}

DWORD GetModuleSize(DWORD modbase) {
	MODULEINFO info = { 0 };
	if (!GetModuleInformation(GetCurrentProcess(), (HMODULE)modbase, &info, sizeof(info))) {
		return 0;
	}
	return info.SizeOfImage;
}

bool GetModNameByFunction(DWORD addr, char* buf, SIZE_T buf_size) {
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (INVALID_HANDLE_VALUE == snap) {
		return false;
	}
	MODULEENTRY32 info = { 0 };
	info.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(snap, &info)) {
		CloseHandle(snap);
		return false;
	}
	do
	{
		DWORD cmp = (DWORD)info.hModule;
		if ((cmp < addr) && addr < (cmp + info.modBaseSize)) {
			RtlCopyMemory(buf, info.szExePath, MAX_PATH);
			CloseHandle(snap);
			return true;
		}
	} while (Module32Next(snap,&info));
	CloseHandle(snap);
	return false;
}

HWND GetHwndByPid(DWORD dwProcessID)
{
	HWND h = GetTopWindow(0);
	HWND retHwnd = NULL;
	while (h)
	{
		DWORD pid = 0;
		if (GetWindowThreadProcessId(h, &pid) != 0){
			if (pid == dwProcessID && GetParent(h) == NULL && ::IsWindowVisible(h)) {
				retHwnd = h;    //会有多个相等值
			}
		}
		h = GetNextWindow(h, GW_HWNDNEXT);
	}
	return retHwnd;
}

bool GetGameWindow(HWND& hwnd)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE){
		return false;
	}
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32)){
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return false;
	}
	do
	{
		if (wcscmp(pe32.szExeFile, L"csgo.exe") == 0) {
			CloseHandle(hProcessSnap);
			hwnd = GetHwndByPid(pe32.th32ProcessID);
			return true;
		}
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	return false;
}