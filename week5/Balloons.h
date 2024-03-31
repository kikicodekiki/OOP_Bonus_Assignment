#pragma once
#include <iostream>
#include "GlobalConstants.h"

struct Balloons {
private:
    enum class BalloonColor {
        GREEN,
        RED,
        YELLOW,
        PINK
    };

    BalloonColor getColorFromStr (const char* str);
    double balloonPrice = 0.0;
    //int balloonsFound [GlobalConstants::NAME_MAX_SIZE] = {0};
//po dobre shte e s bitset

    Balloons() = default;
};
