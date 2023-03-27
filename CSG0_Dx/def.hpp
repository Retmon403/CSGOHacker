#pragma once
#include <Windows.h>
#include <dwmapi.h>
#include <Psapi.h>
#include <d3d9.h>
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <Tlhelp32.h>
#include <process.h> 
#include <Urlmon.h>
#include <io.h>
#include <time.h>


#include "dx/d3dx9.h"
#include "dx/d3dx9anim.h"
#include "dx/d3dx9core.h"
#include "dx/d3dx9effect.h"
#include "dx/d3dx9math.h"
#include "dx/d3dx9mesh.h"
#include "dx/d3dx9shader.h"
#include "dx/d3dx9shape.h"
#include "dx/d3dx9tex.h"
#include "dx/d3dx9xof.h"
#pragma comment (lib , "dx/d3dx9.lib")

#pragma comment (lib , "Urlmon.lib")


#include "VMP/VMProtectSDK.h"
#pragma comment( lib, "VMP/VMProtectSDK32.lib")

#pragma comment( lib, "winmm.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d9.lib")

#include "Detours/detours.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/baidu_font.hpp"

#pragma warning(disable : 4996)






/*
Steam

gameoverlayrenderer.OverlayHookD3D3+3F30 - 55                    - push ebp
gameoverlayrenderer.OverlayHookD3D3+3F31 - 8B EC                 - mov ebp,esp
gameoverlayrenderer.OverlayHookD3D3+3F33 - 83 EC 4C              - sub esp,4C
gameoverlayrenderer.OverlayHookD3D3+3F36 - 53                    - push ebx
gameoverlayrenderer.OverlayHookD3D3+3F37 - 8B 5D 08              - mov ebx,[ebp+08]
gameoverlayrenderer.OverlayHookD3D3+3F3A - 56                    - push esi
gameoverlayrenderer.OverlayHookD3D3+3F3B - 8B 35 A0768777        - mov esi,[gameoverlayrenderer.dll+1076A0]
gameoverlayrenderer.OverlayHookD3D3+3F41 - 57                    - push edi
gameoverlayrenderer.OverlayHookD3D3+3F42 - 33 FF                 - xor edi,edi
gameoverlayrenderer.OverlayHookD3D3+3F44 - 89 5D 08              - mov [ebp+08],ebx
gameoverlayrenderer.OverlayHookD3D3+3F47 - 89 7D FC              - mov [ebp-04],edi
gameoverlayrenderer.OverlayHookD3D3+3F4A - 85 F6                 - test esi,esi
gameoverlayrenderer.OverlayHookD3D3+3F4C - 74 0F                 - je gameoverlayrenderer.OverlayHookD3D3+3F5D
gameoverlayrenderer.OverlayHookD3D3+3F4E - 6A FF                 - push -01
gameoverlayrenderer.OverlayHookD3D3+3F50 - 56                    - push esi
gameoverlayrenderer.OverlayHookD3D3+3F51 - FF 15 74D28377        - call dword ptr [gameoverlayrenderer.dll+CD274]
gameoverlayrenderer.OverlayHookD3D3+3F57 - 8B 35 A0768777        - mov esi,[gameoverlayrenderer.dll+1076A0]
gameoverlayrenderer.OverlayHookD3D3+3F5D - 8D 45 08              - lea eax,[ebp+08]
gameoverlayrenderer.OverlayHookD3D3+3F60 - B9 A4768777           - mov ecx,gameoverlayrenderer.dll+1076A4


*/