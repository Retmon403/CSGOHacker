#include "Draw.hpp"
#include "func.hpp"
#include "Hacker.hpp"
#include "hook.hpp"
#include "style.hpp"

bool g_isShowHide = true;
bool g_isdebug_window = true;
bool g_color_isshow = false;

void Menu(){
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    if (g_isShowHide) {
        SetStyleEnemyMouse();
        if (g_color_isshow) {
            ImGui::SetNextWindowSize(ImVec2(350, 250), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowBgAlpha(0.75f);
            ImGui::Begin(u8"颜色选项");
            ImGui::ColorEdit4(u8"敌人颜色", g_32Col_Obj);
            ImGui::ColorEdit4(u8"盟友颜色", g_32Col_friendObj);
            ImGui::ColorEdit4(u8"血量颜色", g_32Col_Hp);
            ImGui::ColorEdit4(u8"背景颜色", g_32Col_textBack);
            ImGui::ColorEdit4(u8"文本颜色", g_32Col_text);
            ImGui::ColorEdit4(u8"骨骼颜色", g_32Col_bone);
            ImGui::ColorEdit4(u8"射线颜色", g_32Col_line);
            ImGui::ColorEdit4(u8"范围颜色", g_32Col_cir);
            ImGui::End();
        }
        ImGui::SetNextWindowSize(ImVec2(365,550), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.75f);
        ImGui::Begin(u8"哒咩！现在是昕昕魔改版 [HOME] 显示/隐藏", NULL, ImGuiWindowFlags_NoCollapse);
        ImGui::Text(u8"【其它】");
        ImGui::Checkbox(u8"颜色设置", &g_color_isshow);
        ImGui::SameLine();
        if (ImGui::Checkbox(u8"调试窗口", &g_isdebug_window)) {
            SetConsoleStatus(g_isdebug_window);
        }
        ImGui::SameLine();
        ImGui::Checkbox(u8"调试模式", &on_off.g_debug_mode);
        ImGui::Separator();
        ImGui::Text(u8"【绘制系列】");
        ImGui::Checkbox(u8"绘制方框", &on_off.g_Esp);
        ImGui::SameLine();
        ImGui::Checkbox(u8"绘制队友", &on_off.g_EspFriend);
        ImGui::SameLine();
        ImGui::Checkbox(u8"绘制骨骼", &on_off.g_EspBons);
        ImGui::Checkbox(u8"绘制信息", &on_off.g_Espinfo);
        ImGui::SameLine();
        ImGui::Checkbox(u8"辅助线条", &on_off.g_EspLine);
        ImGui::SameLine();
        if (ImGui::Checkbox(u8"d3dhook", &on_off.g_d3dhook)) {
            On_Off_D3dHook(on_off.g_d3dhook);
        }
        ImGui::Separator();
        ImGui::Text(u8"【辅助功能】");
        ImGui::Checkbox(u8"空格连跳", &on_off.g_AutoJump);
        ImGui::SameLine();
        ImGui::Checkbox(u8"急停射击", &on_off.g_AttackStop);
        ImGui::SameLine();
        ImGui::Checkbox(u8"自动开枪", &on_off.g_auto_fire);
        ImGui::SliderInt(u8"开枪前延迟(ms)", &on_off.g_auto_fire_delay_former, 0, 200);
        ImGui::SliderInt(u8"开枪后延迟(ms)", &on_off.g_auto_fire_delay_back, 50, 600);
        ImGui::Checkbox(u8"人物发光", &on_off.g_objlight);
        ImGui::SameLine();
        ImGui::Checkbox(u8"发光队友", &on_off.g_light_t);
        ImGui::SameLine();
        ImGui::Checkbox(u8"雷达透视", &on_off.g_radar);

        ImGui::Separator();
        ImGui::Text(u8"【自瞄系列】");
        if (ImGui::Checkbox(u8"自动瞄准", &on_off.g_AutoAim) && on_off.g_AutoAim) {
            on_off.g_AimLock_Memory = false;
            on_off.g_AimLock = true;
        }
        ImGui::SliderInt(u8"自瞄速度", &on_off.g_AutoAimSpeed, 1, 30);
        ImGui::SliderInt(u8"自瞄半径", &on_off.g_AutoAimR, 1, 800);
        ImGui::SliderFloat(u8"自瞄微调", &g_AutoRecoilForceVal, 0.5f, 1.0f);
        ImGui::Checkbox(u8"自瞄队友", &on_off.g_AutoAimFriend);
        ImGui::SameLine();
        if (ImGui::Checkbox(u8"模拟鼠标死锁", &on_off.g_AimLock))
            on_off.g_AimLock_Memory = false;
        ImGui::SameLine();
        if (ImGui::Checkbox(u8"内存暴力死锁", &on_off.g_AimLock_Memory))
            on_off.g_AimLock = false;
        ImGui::Checkbox(u8"自瞄压枪", &on_off.g_AutoRecoilForce);
        //ImGui::SameLine();
        /*if (ImGui::Checkbox(u8"无后坐力", &on_off.g_no_recoil)) {
            SetNoRecoil(on_off.g_no_recoil);
        }*/
        static int curIndex = 2;
        const char* items[] = { u8"左键", u8"右键" ,u8"ALT" , u8"SHIFT" ,u8"CTRL" };
        ImGui::Combo(u8"自瞄按键", &curIndex, items, 5);
        static int AutoAimKey[] = { VK_LBUTTON ,VK_RBUTTON ,VK_MENU ,VK_LSHIFT ,VK_CONTROL };
        on_off.g_AutoAimKey = AutoAimKey[curIndex];

        static int curIndex2 = 0;
        const char* items2[] = { u8"头部" , u8"胸部" , u8"腰部" };
        ImGui::Combo(u8"自瞄位置", &curIndex2, items2, 3);
        static int AutoAimBody[] = { 5,3,0 };
        on_off.g_AutoAimBody = AutoAimBody[curIndex2];
    }


    ESP();
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void MineImGuiInit(IDirect3DDevice9* pd3dDevice)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    io.WantSaveIniSettings = false;
    io.IniFilename = NULL;
    ImFontConfig f_cfg;
    f_cfg.FontDataOwnedByAtlas = false;
    const ImFont* font = io.Fonts->AddFontFromMemoryTTF((void*)baidu_font_data, baidu_font_size, 16.0f, &f_cfg, io.Fonts->GetGlyphRangesChineseFull());
    ImGui_ImplWin32_Init(g_hwnd);
    ImGui_ImplDX9_Init(pd3dDevice);
    ImGui::GetIO().ImeWindowHandle = g_hwnd;
}