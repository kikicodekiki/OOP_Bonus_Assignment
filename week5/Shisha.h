#pragma once
#include <iostream>
#include "GlobalConstants.h"

using GlobalConstants::EPS;

enum class ShishaType {
    OS_QUEEN,
    TROPICAL,
    MINT
};

ShishaType getTypeFromStr (const char* str) {
    if(!str) {
        return {};
    }

    if (strcmp("OS-queen", str) == 0) {
        return ShishaType::OS_QUEEN;
    }

    if (strcmp("tropical", str) == 0) {
        return ShishaType::TROPICAL;
    }

    if (strcmp("mint", str) == 0) {
        return ShishaType::MINT;
    }

}

struct Shisha {
private:

    ShishaType name = ShishaType::OS_QUEEN;
    double price = 0.0;

public:
    Shisha() = default;

    void setShishaType (const char* newName) {
        if(!newName) {
            return;
        }

        name = getTypeFromStr(newName);
    }

    void setShishaPrice (double newPrice) {
        if (newPrice < EPS || std::abs(newPrice - price) < EPS) {
            return;
        }

        price = newPrice;
    }

    ShishaType getShishaType () const {
        return name;
    }

    double getPrice () const {
        return price;
    }
};
