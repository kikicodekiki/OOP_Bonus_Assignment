#pragma once
#include <iostream>
#include "GlobalConstants.h"
#include "Baloons.h"
#include "Narga.h"
#include "Alchohol.h"

using namespace GlobalConstants;

class NightClub {
private:
    size_t curAlcoholCount = 0;
    size_t curShishaCount = 0;
    size_t curBalloonCount = 0;

    Alcohol alcohol[ALCOHOL_MAX_NUMBER] = {};
    Shisha shisha[NARGAS_MAX_NUMBER] = {};
    Balloons balloons[BALLOONS_MAX_NUMBER] = {};

    //reshih che mai bez bitset shte e po ok

    int balloonColors [4] = {0}; // 0 - green 1 - red 2-yellow 3-pink
    int alcoholBottles [4] = {0}; // 0 - blackLabel 1- jameson 2 - moet 3 - don julio
    int shishaTypes [3] = {0}; // 0 - osQueen 1 - tropical 2 - mint

public:
    NightClub() = default;

    void addBalloon(const char* color, double price) {
        if (!color || price < EPS ) {
            return;
        }

        if (curBalloonCount < BALLOONS_MAX_NUMBER) {
            balloons[curBalloonCount].setBalloonColor(color);
            balloons[curBalloonCount].setBalloonPrice(price);
            int myColor = (int)(balloons[curBalloonCount].getBalloonColor());
            balloonColors[myColor]++;
            curBalloonCount++;
        }
    }

    void addAlcohol (const char* name, double price) {
        if (!name || price < EPS) {
            return;
        }

        if (curAlcoholCount < ALCOHOL_MAX_NUMBER) {
            //ne e dobra abstrakciq :/
            alcohol[curAlcoholCount].setAlchoholName(name);
            alcohol[curAlcoholCount].setAlchoholPrice(price);
            int myName = (int)(alcohol[curAlcoholCount].getAlcoholName());
            alcoholBottles[myName]++;
            curAlcoholCount++;
        }
    }

    void addShisha (const char* type, double price) {
        if (!type || price < EPS) {
            return;
        }

        if (curShishaCount < NARGAS_MAX_NUMBER) {
            shisha[curShishaCount].setShishaType(type);
            shisha[curShishaCount].setShishaPrice(price);
            int myType = (int)(shisha[curShishaCount].getShishaType());
            shishaTypes[myType]++;
            curShishaCount++;
        }
    }

};
