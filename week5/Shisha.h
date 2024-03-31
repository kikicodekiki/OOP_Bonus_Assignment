#pragma once
#include <iostream>
#include "GlobalConstants.h"

using GlobalConstants::EPS;

struct Shisha {
private:
    enum class ShishaType {
        OS_QUEEN,
        TROPICAL,
        MINT
    };

    ShishaType name = ShishaType::OS_QUEEN;
    double price = 0.0;

    ShishaType getTypeFromStr (const char* str) {
        if(!str) {
            return {};
        }

        if (strcmp("OS-queen", str) == 0) {
            return name = ShishaType::OS_QUEEN;
        }

        if (strcmp("tropical", str) == 0) {
            return name = ShishaType::TROPICAL;
        }

        if (strcmp("mint", str) == 0) {
            return name = ShishaType::MINT;
        }

    }

public:
    Shisha() = default;

    void setShishaType (const char* newName) {
        if(!newName) {
            return;
        }

        getTypeFromStr(newName);
    }

    void setAlchoholPrice (double newPrice) {
        if (newPrice < EPS || std::abs(newPrice - price) < EPS) {
            return;
        }

        price = newPrice;
    }
};
