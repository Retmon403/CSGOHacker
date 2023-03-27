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
            ImGui::Begin(u8"��ɫѡ��");
            ImGui::ColorEdit4(u8"������ɫ", g_32Col_Obj);
            ImGui::ColorEdit4(u8"������ɫ", g_32Col_friendObj);
            ImGui::ColorEdit4(u8"Ѫ����ɫ", g_32Col_Hp);
            ImGui::ColorEdit4(u8"������ɫ", g_32Col_textBack);
            ImGui::ColorEdit4(u8"�ı���ɫ", g_32Col_text);
            ImGui::ColorEdit4(u8"������ɫ", g_32Col_bone);
            ImGui::ColorEdit4(u8"������ɫ", g_32Col_line);
            ImGui::ColorEdit4(u8"��Χ��ɫ", g_32Col_cir);
            ImGui::End();
        }
        ImGui::SetNextWindowSize(ImVec2(365,550), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.75f);
        ImGui::Begin(u8"���㣡��������ħ�İ� [HOME] ��ʾ/����", NULL, ImGuiWindowFlags_NoCollapse);
        ImGui::Text(u8"��������");
        ImGui::Checkbox(u8"��ɫ����", &g_color_isshow);
        ImGui::SameLine();
        if (ImGui::Checkbox(u8"���Դ���", &g_isdebug_window)) {
            SetConsoleStatus(g_isdebug_window);
        }
        ImGui::SameLine();
        ImGui::Checkbox(u8"����ģʽ", &on_off.g_debug_mode);
        ImGui::Separator();
        ImGui::Text(u8"������ϵ�С�");
        ImGui::Checkbox(u8"���Ʒ���", &on_off.g_Esp);
        ImGui::SameLine();
        ImGui::Checkbox(u8"���ƶ���", &on_off.g_EspFriend);
        ImGui::SameLine();
        ImGui::Checkbox(u8"���ƹ���", &on_off.g_EspBons);
        ImGui::Checkbox(u8"������Ϣ", &on_off.g_Espinfo);
        ImGui::SameLine();
        ImGui::Checkbox(u8"��������", &on_off.g_EspLine);
        ImGui::SameLine();
        if (ImGui::Checkbox(u8"d3dhook", &on_off.g_d3dhook)) {
            On_Off_D3dHook(on_off.g_d3dhook);
        }
        ImGui::Separator();
        ImGui::Text(u8"���������ܡ�");
        ImGui::Checkbox(u8"�ո�����", &on_off.g_AutoJump);
        ImGui::SameLine();
        ImGui::Checkbox(u8"��ͣ���", &on_off.g_AttackStop);
        ImGui::SameLine();
        ImGui::Checkbox(u8"�Զ���ǹ", &on_off.g_auto_fire);
        ImGui::SliderInt(u8"��ǹǰ�ӳ�(ms)", &on_off.g_auto_fire_delay_former, 0, 200);
        ImGui::SliderInt(u8"��ǹ���ӳ�(ms)", &on_off.g_auto_fire_delay_back, 50, 600);
        ImGui::Checkbox(u8"���﷢��", &on_off.g_objlight);
        ImGui::SameLine();
        ImGui::Checkbox(u8"�������", &on_off.g_light_t);
        ImGui::SameLine();
        ImGui::Checkbox(u8"�״�͸��", &on_off.g_radar);

        ImGui::Separator();
        ImGui::Text(u8"������ϵ�С�");
        if (ImGui::Checkbox(u8"�Զ���׼", &on_off.g_AutoAim) && on_off.g_AutoAim) {
            on_off.g_AimLock_Memory = false;
            on_off.g_AimLock = true;
        }
        ImGui::SliderInt(u8"�����ٶ�", &on_off.g_AutoAimSpeed, 1, 30);
        ImGui::SliderInt(u8"����뾶", &on_off.g_AutoAimR, 1, 800);
        ImGui::SliderFloat(u8"����΢��", &g_AutoRecoilForceVal, 0.5f, 1.0f);
        ImGui::Checkbox(u8"�������", &on_off.g_AutoAimFriend);
        ImGui::SameLine();
        if (ImGui::Checkbox(u8"ģ���������", &on_off.g_AimLock))
            on_off.g_AimLock_Memory = false;
        ImGui::SameLine();
        if (ImGui::Checkbox(u8"�ڴ汩������", &on_off.g_AimLock_Memory))
            on_off.g_AimLock = false;
        ImGui::Checkbox(u8"����ѹǹ", &on_off.g_AutoRecoilForce);
        //ImGui::SameLine();
        /*if (ImGui::Checkbox(u8"�޺�����", &on_off.g_no_recoil)) {
            SetNoRecoil(on_off.g_no_recoil);
        }*/
        static int curIndex = 2;
        const char* items[] = { u8"���", u8"�Ҽ�" ,u8"ALT" , u8"SHIFT" ,u8"CTRL" };
        ImGui::Combo(u8"���鰴��", &curIndex, items, 5);
        static int AutoAimKey[] = { VK_LBUTTON ,VK_RBUTTON ,VK_MENU ,VK_LSHIFT ,VK_CONTROL };
        on_off.g_AutoAimKey = AutoAimKey[curIndex];

        static int curIndex2 = 0;
        const char* items2[] = { u8"ͷ��" , u8"�ز�" , u8"����" };
        ImGui::Combo(u8"����λ��", &curIndex2, items2, 3);
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