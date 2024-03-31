#pragma once
#include <iostream>

using GlobalConstants::NAME_MAX_SIZE;

struct Client {
private:
    char* clientName;
    char fileName[NAME_MAX_SIZE];
    //dinamichna pamet -> big 4

    void copyFrom(const Client& data);
    void free();

public:
    Client();
    ~Client();
    Client (const Client& other);
    Client& operator=(const Client& other);
};
