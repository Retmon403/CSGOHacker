#pragma once
#include "def.hpp"

//-allow_third_party_software
typedef HRESULT(WINAPI* Reset_t)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*); // 16
typedef HRESULT(WINAPI* EndScene_t)(IDirect3DDevice9*);                      // 42
typedef LRESULT(WINAPI* WndProc_t)(const HWND, UINT, WPARAM, LPARAM);
typedef HRESULT(WINAPI* Present)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
typedef HRESULT(WINAPI* DrawIndexedPrimitive)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
typedef HRESULT(WINAPI* SetVertexDeclaration)(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);

DWORD WINAPI init(LPVOID lpThreadParameter);
void On_Off_D3dHook(bool ishk);
extern HWND g_hwnd;