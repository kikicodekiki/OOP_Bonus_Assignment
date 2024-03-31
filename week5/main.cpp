#pragma once
#include <iostream>
#include "NightClub.h"
#include "Client.h"

using namespace GlobalConstants;

double calculateDailyProfitWithFile(Client clients[], int numberOfClients, NightClub& club, const char* fileName) {
    std::ofstream ofs (fileName, std::ios::out|std::ios::binary);

    double dailyProfit = 0.0;

    if (ofs.is_open()) {
        for (int i = 0; i < numberOfClients; ++i) {
            int size = strlen(clients[i].getClientName()) + 1;
            ofs.write(clients[i].getClientName(), size );

            double profit = club.returnBillFromFile(clients[i].getFileName());
            ofs.write((const char*)&profit, sizeof(double));

            dailyProfit += profit;

        }

        ofs.write((const char*)&dailyProfit, sizeof (double));

    }

    return dailyProfit;

}

double calculateDailyProfit (Client clients[], int numberOfClients, NightClub& club) {
    double dailyProfit = 0.0;

    for (int i = 0; i < numberOfClients; ++i) {

        double profit = club.returnBill();

        dailyProfit += profit;
    }
    return dailyProfit;
}

int main() {

    NightClub club;

    //adding inventory
    club.addAlcohol("black_label", 2.50);
    club.addBalloon("green", 10.30);
    club.addShisha("OS-queen", 2.80);

    // проверка на наличност
    std::cout << "Has green balloon: " << club.hasBalloon("green") << std::endl;
    std::cout << "Has black label alcohol: " << club.hasAlcohol("black_label") << std::endl;
    std::cout << "Has OS queen shisha: " << club.hasNargile("OS-queen") << std::endl;

    Client clients[1] = { {"John", "john.txt"} };
    std::cout << "The bill is: ";
    std::cout << calculateDailyProfit(clients, 1, club);

}
