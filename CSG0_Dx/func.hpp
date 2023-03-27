#pragma once
#include "def.hpp"

void SetConsoleStatus(bool iscreate);
DWORD FindPattern(DWORD dwAddress, DWORD dwLength, const char* szPattern, DWORD length);
DWORD GetModuleSize(DWORD modbase);
bool GetModNameByFunction(DWORD addr, char* buf, SIZE_T buf_size);
bool GetGameWindow(HWND& hwnd);