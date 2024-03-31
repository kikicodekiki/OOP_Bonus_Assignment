#pragma once
#include <iostream>
#include "GlobalConstants.h"
#include "Baloons.h"
#include "Narga.h"
#include "Alchohol.h"
#include <fstream>
#include "Client.h"
#include "isSubstring.h"

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

    void readAlcoholFromFile (std::ifstream& in) {
        char name[NAME_MAX_SIZE];
        in.getline(name, NAME_MAX_SIZE, '-');
        double price;
        in>>price;
        addAlcohol(name, price);
    }

    void readBalloonFromFile (std::ifstream& in) {
        char name[NAME_MAX_SIZE];
        in.getline(name, NAME_MAX_SIZE, '-');
        double price;
        in>>price;
        addBalloon(name, price);
    }

    void readShishaFromFile (std::ifstream& in) {
        char name[NAME_MAX_SIZE];
        in.getline(name, NAME_MAX_SIZE, '-');
        double price;
        in>>price;
        addShisha(name, price);
    }

    bool hasBalloon(const char* color) {

        if(!color) {
            return false;
        }

       int newColor = (int) (getColorFromStr(color));
       if (balloonColors[newColor] > 0) {
           return true;
       }
       return false;
    }

    bool hasAlcohol(const char* name) {

        if(!name) {
            return false;
        }

        int newName = (int) (getNameFromStr(name));
        if (alcoholBottles[newName] > 0) {
            return true;
        }
        return false;
    }

    bool hasNargile( const char* flavor) {
        if(!flavor) {
            return false;
        }

        int newType = (int) (getTypeFromStr(flavor));
        if (shishaTypes[newType] > 0) {
            return true;
        }
        return false;
    }

    void removeBalloon(const char* color) {
        if(!color) {
            return;
        }

        int myColor = (int)(getColorFromStr(color));
        if (myColor < 1) {
            return;
        }

        for (size_t i = 0; i < curBalloonCount; ++i) {
            if (balloons[i].getBalloonColor() == getColorFromStr(color)) {
                for (size_t j = i; j < curBalloonCount - 1; ++j) {
                    balloons[j] = balloons[j + 1];
                }
                curBalloonCount--;
                balloonColors[myColor]--;
                break;
            }
        }

    }

    void removeAlcohol(const char* name) {
        if(!name) {
            return;
        }

        int myName = (int)(getNameFromStr(name));
        if (myName < 1) {
            return;
        }

        for (size_t i = 0; i < curAlcoholCount; ++i) {
            if (alcohol[i].getAlcoholName() == getNameFromStr(name)) {
                for (size_t j = i; j < curAlcoholCount - 1; ++j) {
                    alcohol[j] = alcohol[j + 1];
                }
                curAlcoholCount--;
                balloonColors[myName]--;
                break;
            }
        }
    }

    void removeShisha( const char* flavor) {
        if(!flavor) {
            return;
        }

        int myType = (int)(getTypeFromStr(flavor));
        if (myType < 1) {
            return;
        }

        for (size_t i = 0; i < curShishaCount; ++i) {
            if (shisha[i].getShishaType() == getTypeFromStr(flavor)) {
                for (size_t j = i; j < curShishaCount - 1; ++j) {
                    shisha[j] = shisha[j + 1];
                }
                curShishaCount--;
                shishaTypes[myType]--;
                break;
            }
        }
    }

    double returnBill(const char* fileName) {
        if (!fileName) {
            return -1.0;
        }

        std::ifstream in(fileName);
        if (!in.is_open()) {
            return -1;
        }

        double totalBill = 0.0;
        char buff[1024];

        while (!in.eof()) {
            in.getline(buff, 1024, ':');
            if (isSubstring(buff, "Alcohols") != -1) {
                //predpolagame che sa podredeni alcohol/baloni/nargileta
                while (in.peek() != 'B') {
                    //tova s peek mi go pokazaha edni ot vtori kurs na konsultaciqta
                    // nz dali mozhe da go polzvam no iskah
                    //da go probvam :) *mazna usmivka*
                    readAlcoholFromFile(in);
                }
            } else if (isSubstring(buff, "Baloons") != -1) {

                while (in.peek() != 'N') {
                    readBalloonFromFile(in);
                }
            } else if (isSubstring(buff, "Nargas") != -1) {

                while (!in.eof()) {
                    readShishaFromFile(in);
                }
            }
        }

        
        for (size_t i = 0; i < curAlcoholCount; ++i) {
            totalBill += alcohol[i].getPrice();
        }

        for (size_t i = 0; i < curBalloonCount; ++i) {
            totalBill += balloons[i].getPrice();
        }

        for (size_t i = 0; i < curShishaCount; ++i) {
            totalBill += shisha[i].getPrice();
        }

        return totalBill;
    }


};
