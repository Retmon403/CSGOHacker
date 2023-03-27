#pragma once
#include "def.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


extern bool g_isShowHide;


void MineImGuiInit(IDirect3DDevice9* pd3dDevice);
void Menu();
void DrawLine(float x, float y, float xx, float yy, ImU32 color);// 绘制直线
void DrawBox(float x, float y, float width, float height, ImU32 color);// 绘制方框
void DrawRound(float x, float y, float radius /* 半径 */, ImU32 Color); // 绘制圆