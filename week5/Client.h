#pragma once
#include <iostream>

using GlobalConstants::NAME_MAX_SIZE;

struct Client {
private:
    char* clientName = nullptr;
    char fileName[NAME_MAX_SIZE] = "unknown";
    //dinamichna pamet -> big 4

    void copyFrom(const Client& other) {
        int size = strlen(other.clientName) + 1;
        clientName = new char [size];
        strcpy (clientName, other.clientName);
        strcpy(fileName, other.fileName);
    }

    void free() {
        delete[] clientName;
        clientName = nullptr;
    }

public:
    Client() = default;

    Client (const char* newClient, const char* newFile) {
        if(!newClient || !newFile) {
            Client();
        }
        setClientName(newClient);
        setFileName(newFile);
    }

    ~Client() {
        free();
    }
    Client (const Client& other) {
        copyFrom(other);
    }
    Client& operator=(const Client& other) {
        if(this != &other) {
            free();
            copyFrom(other);
        }
        return *this;
    }
    const char* getClientName () const {
        return clientName;
    }
    const char* getFileName () const {
        return fileName;
    }

    void setClientName (const char* newName) {
        if(!newName) {
            return;
        }

        int size = strlen(newName) + 1;
        clientName = new char [size];
        strcpy (clientName, newName);
    }

    void setFileName (const char* newName) {
        if(!newName) {
            return;
        }
        strcpy(fileName, newName);
    }
};
