#include "hook.hpp"
#include "func.hpp"
#include "Draw.hpp"
#include "Hacker.hpp"
#include "sdk.hpp"

SetVertexDeclaration oSetVertexDeclaration;
DrawIndexedPrimitive oDrawIndexedPrimitive;
Reset_t     oReset;
EndScene_t  oEndScene;
Present oPresent;
WndProc_t   oWndProc;

HWND g_hwnd;
D3DVERTEXELEMENT9 decl[MAXD3DDECLLENGTH];
UINT g_numElements;

void HookFunction(PVOID* oFunction, PVOID Function, bool ishk)
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    ishk ? DetourAttach(oFunction, Function) : DetourDetach(oFunction, Function);
    DetourTransactionCommit();
}

LRESULT WINAPI hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    if (msg == WM_KEYUP){
        if (wParam == VK_HOME) {
            g_isShowHide = !g_isShowHide;
            goto end;
        }
    }
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }
    end:
    return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}

HRESULT WINAPI hkEndScene(IDirect3DDevice9* pd3dDevice) {
    static bool firstCall = true;
    if (firstCall) {
        firstCall = false;
        MineImGuiInit(pd3dDevice);
        oWndProc = (WNDPROC)SetWindowLongPtr(g_hwnd, GWL_WNDPROC, (LONG_PTR)hkWndProc);
    }
    Menu();
    return oEndScene(pd3dDevice);
}

HRESULT WINAPI hkPresent(IDirect3DDevice9* pd3dDevice, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region){
    static bool firstCall = true;
    if (firstCall) {
        firstCall = false;
        MineImGuiInit(pd3dDevice);
        oWndProc = (WNDPROC)SetWindowLongPtr(g_hwnd, GWL_WNDPROC, (LONG_PTR)hkWndProc);
    }
    Menu();
    return oPresent(pd3dDevice, src, dest, wnd_override, dirty_region);
}

HRESULT WINAPI hkReset(IDirect3DDevice9* pd3dDevice, D3DPRESENT_PARAMETERS* pPresentationParameters){
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT result = oReset(pd3dDevice, pPresentationParameters);
    ImGui_ImplDX9_CreateDeviceObjects();
    return result;
}

HRESULT WINAPI hkDrawIndexedPrimitive(IDirect3DDevice9* pd3dDevice,D3DPRIMITIVETYPE unnamedParam1, INT BaseVertexIndex,
    UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
    if (g_numElements == 9 || g_numElements == 12) {
        pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
        oDrawIndexedPrimitive(pd3dDevice, unnamedParam1, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
        pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    }
    return oDrawIndexedPrimitive(pd3dDevice, unnamedParam1, BaseVertexIndex, MinVertexIndex, NumVertices,startIndex, primCount);
}

HRESULT WINAPI hkSetVertexDeclaration(IDirect3DDevice9* pd3dDevice, IDirect3DVertexDeclaration9* pdecl) {
    if (pdecl) {
        pdecl->GetDeclaration(decl, &g_numElements);
    }
    return oSetVertexDeclaration(pd3dDevice, pdecl);
}

bool HookStart() {
    DWORD csgopluginbase = 0, steamoverlay = 0, obs_base = 0, hookpos = 0;
    while (!steamoverlay){
        steamoverlay = (DWORD)GetModuleHandleA("GameOverlayRenderer.dll");
        Sleep(100);
    }
    ::printf("SteamModBase -> GameOverlayRenderer:%p\n", steamoverlay);
    //obs_base = (DWORD)GetModuleHandleA("graphics-hook32.dll");
    //SetWindowDisplayAffinity(g_hwnd, WDA_MONITOR);
    if (*(PCHAR)oPresent == (CHAR)0xe9) {
        ULONG jmp = *(PULONG)(((PCHAR)oPresent) + 1);
        oPresent = (Present)(jmp + (ULONG)oPresent + 5);
    }
    else {
        printf("find steamHookjump failed!\n");
        return false;
    }
    HookFunction((PVOID*)(&oReset), hkReset, true);
    HookFunction((PVOID*)(&oPresent), hkPresent, true);
    CloseHandle(CreateThread(NULL, 0, ThreadLoops, NULL, 0, NULL));
    CloseHandle(CreateThread(NULL, 0, ThreadLoops_Funny, NULL, 0, NULL));
    CloseHandle(CreateThread(NULL, 0, ThreadLoops_AutoFire, NULL, 0, NULL));
    return true;
}

void On_Off_D3dHook(bool ishk) {
    HookFunction((PVOID*)(&oSetVertexDeclaration), hkSetVertexDeclaration, ishk);
    HookFunction((PVOID*)(&oDrawIndexedPrimitive), hkDrawIndexedPrimitive, ishk);
}

bool Dx9Init() {

    IDirect3D9* g_Direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp = { 0 };
    IDirect3DDevice9* g_pd3dDevice;
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    HRESULT result = g_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);
    if (result != D3D_OK) {
        g_Direct3D9->Release();
        return false;
    }
    PDWORD dDeviceVT = (PDWORD) * (PDWORD)g_pd3dDevice;
    oReset = (Reset_t)dDeviceVT[16];
    oPresent = (Present)dDeviceVT[17];
    oEndScene = (EndScene_t)dDeviceVT[42];
    oDrawIndexedPrimitive = (DrawIndexedPrimitive)dDeviceVT[82];
    oSetVertexDeclaration = (SetVertexDeclaration)dDeviceVT[87];
    g_pd3dDevice->Release();
    g_Direct3D9->Release();
    ::printf("Reset:%p oPresent:%p EndScene:%p\nDrawIndexedPrimitive:%p SetVertexDeclaration:%p\n", oReset, oPresent,
        oEndScene, oDrawIndexedPrimitive, oSetVertexDeclaration);
    return true;
}


DWORD WINAPI init(LPVOID lpThreadParameter) {
    while (!GetGameWindow(g_hwnd) || !ListProcessModules()) {
        Sleep(100);
    }
    ::printf("MyhModule:%p gHwnd:%ld client:%p engine:%p\n", lpThreadParameter, g_hwnd, g_dwClientAdr, g_dwEngineAdr);
    LoadConfig();
    Dx9Init();
    ::printf(HookStart() ? "hook success\n" : "hook failed\n");
    if (!FindInterface()) {
        printf("FindInterface failed!\n");
    }
    
    return 0;
}




BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
#if _DEBUG
#else if _RELEASE
        memset(hModule, 0, 1000);
#endif
        DisableThreadLibraryCalls(hModule);
        SetConsoleStatus(true);
        CloseHandle(CreateThread(NULL, 0, init, (LPVOID)hModule, 0, NULL));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}