#pragma once

#include "def.hpp"

enum SelectStyle {
    StyleRed,
    StyleGreen,
    StyleBlue,
    StyleOrange
};

void SetSolidColorStyle(SelectStyle);
void SetDarkTheme();
void SetStyleEnemyMouse();