#pragma once
#include "def.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


extern bool g_isShowHide;


void MineImGuiInit(IDirect3DDevice9* pd3dDevice);
void Menu();
void DrawLine(float x, float y, float xx, float yy, ImU32 color);// ����ֱ��
void DrawBox(float x, float y, float width, float height, ImU32 color);// ���Ʒ���
void DrawRound(float x, float y, float radius /* �뾶 */, ImU32 Color); // ����Բ