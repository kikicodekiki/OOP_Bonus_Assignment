#pragma once
#include <iostream>
#include "GlobalConstants.h"
using namespace GlobalConstants;

struct Balloons {
private:
    enum class BalloonColor {
        GREEN,
        RED,
        YELLOW,
        PINK
    };

    BalloonColor color = BalloonColor::GREEN;
    double price = 0.0;
    int balloonsFound [GlobalConstants::NAME_MAX_SIZE] = {0};

    BalloonColor getColorFromStr (const char* str) {
        if(!str) {
            return {};
        }

        if (strcmp("green", str) == 0) {
            return color = BalloonColor::GREEN;
        }

        if (strcmp("red", str) == 0) {
            return color = BalloonColor::RED;
        }

        if (strcmp("yellow", str) == 0) {
            return color = BalloonColor::YELLOW;
        }

        if (strcmp("pink", str) == 0) {
            return color = BalloonColor::PINK;
        }
    }

public:
    Balloons() = default;

    void setBalloonColor (const char* colorName) {
        if (!colorName) {
            return;
        }

        getColorFromStr(colorName);
    }
    void setBalloonPrice (double newPrice) {
        if (newPrice < EPS || std::abs(newPrice - price) < EPS) {
            return;
        }

        price = newPrice;
    }
};
