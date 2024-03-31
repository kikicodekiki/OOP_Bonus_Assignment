#pragma once
#include <iostream>
#include "GlobalConstants.h"

using namespace GlobalConstants;

class NightClub {
private:
    size_t curAlcoholCount = 0;
    size_t curShishaCount = 0;
    size_t curBalloonCount = 0;

    //reshih che mai bez bitset shte e po ok 

    int balloonColors [4] = {0}; // 0 - green 1 - red 2-yellow 3-pink
    int alcoholBottles [4] = {0}; // 0 - blackLabel 1- jameson 2 - moet 3 - don julio
    int shishaTypes [3] = {0}; // 0 - osQueen 1 - tropical 2 - mint


};
