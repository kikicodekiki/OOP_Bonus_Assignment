#pragma once
#include "GlobalConstants.h"

using namespace GlobalConstants;

enum class AlcoholName {
    MOET,
    JAMESON,
    BLACK_LABEL,
    DON_JULIO_1942
};

AlcoholName getNameFromStr (const char* str) {
    if(!str) {
        return {};
    }

    if (strcmp("black_label", str) == 0) {
        return AlcoholName::BLACK_LABEL;
    }

    if (strcmp("jameson", str) == 0) {
        return  AlcoholName::JAMESON;
    }

    if (strcmp("moet", str) == 0) {
        return AlcoholName::MOET;
    }

    if (strcmp("don_julio_1942", str) == 0) {
        return AlcoholName::DON_JULIO_1942;
    }
}

struct Alcohol {
private:

    AlcoholName name = AlcoholName::MOET;
    double price = 0.0;

public:
    Alcohol() = default;

    void setAlchoholName (const char* newName) {
        if(!newName) {
            return;
        }

        name = getNameFromStr(newName);
    }

    void setAlchoholPrice (double newPrice) {
        if (newPrice < EPS || std::abs(newPrice - price) < EPS) {
            return;
        }

        price = newPrice;
    }

    AlcoholName getAlcoholName () const {
        return name;
    }

    double getPrice () const {
        return price;
    }
};
