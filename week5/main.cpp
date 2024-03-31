#pragma once
#include <iostream>
#include "NightClub.h"
#include "Client.h"

using namespace GlobalConstants;

double calculateDailyProfit(Client clients[], int numberOfClients, NightClub& club, const char* fileName) {
    std::ofstream ofs (fileName, std::ios::out|std::ios::binary);

    double dailyProfit = 0.0;

    if (ofs.is_open()) {
        for (int i = 0; i < numberOfClients; ++i) {
            int size = strlen(clients[i].getClientName()) + 1;
            ofs.write(clients[i].getClientName(), size );

            double profit = club.returnBill(clients[i].getFileName());
            ofs.write((const char*)&profit, sizeof(double));

            dailyProfit += profit;

        }

        ofs.write((const char*)&dailyProfit, sizeof (double));

    }

    return dailyProfit;

}

int main() {

    NightClub club;


}
