#pragma once
#include "GlobalConstants.h"

using namespace GlobalConstants;

struct Alcohol {
private:
    enum class AlcoholName {
        MOET,
        JAMESON,
        BLACK_LABEL,
        DON_JULIO_1942
    };

    AlcoholName name = AlcoholName::MOET;
    double price = 0.0;

    AlcoholName getColorFromStr (const char* str) {
        if(!str) {
            return {};
        }

        if (strcmp("black_label", str) == 0) {
            return name = AlcoholName::BLACK_LABEL;
        }

        if (strcmp("jameson", str) == 0) {
            return name = AlcoholName::JAMESON;
        }

        if (strcmp("moet", str) == 0) {
            return name = AlcoholName::MOET;
        }

        if (strcmp("don_julio_1942", str) == 0) {
            return name = AlcoholName::DON_JULIO_1942;
        }
    }

public:
    Alcohol() = default;

    void setAlchoholName (const char* newName) {
        if(!newName) {
            return;
        }

        getColorFromStr(newName);
    }

    void setAlchoholPrice (double newPrice) {
        if (newPrice < EPS || std::abs(newPrice - price) < EPS) {
            return;
        }

        price = newPrice;
    }
};
