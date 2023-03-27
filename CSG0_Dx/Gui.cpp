#include "Gui.hpp"

void DrawLine(float x, float y, float xx, float yy, ImU32 color) // 绘制直线
{
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(xx, yy), color); // 绘制线条
}
void DrawBox(float x, float y, float width, float height, ImU32 color) // 绘制矩形
{
    ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + width, y + height), color, 0.0f, ImDrawCornerFlags_All, 3.f);
}
void DrawRound(float x, float y, float radius /* 半径 */, ImU32 Color)
{
    ImGui::GetForegroundDrawList()->AddCircle(ImVec2(x, y), radius, Color);
}
void DrawBox2(float x, float y, float w, float h, ImU32 color, float thickness)
{
    float w1 = 0.f, h1 = 0.f;
    w1 = w / 3.f;
    h1 = h / 4.f;
    // 左上角
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x + w1, y), color, thickness);
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x, y + h1), color, thickness);

    // 右上角
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + 2.f * w1, y), ImVec2(x + w, y), color, thickness);
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + h1), color, thickness);

    // 左下角
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + 3.f * h1), ImVec2(x, y + h), color, thickness);
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x + w1, y + h), color, thickness);

    // 右下角
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w1 * 2.f, y + h), ImVec2(x + w, y + h), color, thickness);
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y + h), ImVec2(x + w, y + 3.f * h1), color, thickness);
}