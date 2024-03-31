#pragma once
#include <iostream>
#include "GlobalConstants.h"
using namespace GlobalConstants;

enum class BalloonColor {
    GREEN,
    RED,
    YELLOW,
    PINK
};

BalloonColor getColorFromStr (const char* str) {
    if(!str) {
        return {};
    }

    if (strcmp("green", str) == 0) {
        return BalloonColor::GREEN;
    }

    if (strcmp("red", str) == 0) {
        return BalloonColor::RED;
    }

    if (strcmp("yellow", str) == 0) {
        return BalloonColor::YELLOW;
    }

    if (strcmp("pink", str) == 0) {
        return BalloonColor::PINK;
    }
}

struct Balloons {
private:

    BalloonColor color = BalloonColor::GREEN;
    double price = 0.0;

public:
    Balloons() = default;

    void setBalloonColor (const char* colorName) {
        if (!colorName) {
            return;
        }

        color = getColorFromStr(colorName);
    }
    void setBalloonPrice (double newPrice) {
        if (newPrice < EPS || std::abs(newPrice - price) < EPS) {
            return;
        }

        price = newPrice;
    }

    BalloonColor getBalloonColor () const {
        return color;
    }

    double getPrice () const {
        return price;
    }
};
