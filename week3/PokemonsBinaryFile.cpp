#include <iostream>
#include <fstream>
#include <cstring>

namespace GlobalConstants {
    constexpr int NAME_MAX_SIZE = 50;
    constexpr int POWER_MIN_SIZE = 10;
    constexpr int POWER_MAX_SIZE = 1000;
    constexpr int TYPE_MAX_LENGHT = 10;
}

enum Type {
    NORMAL,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    GHOST,
    FLYING
};

struct Pokemon {
    char name [GlobalConstants::NAME_MAX_SIZE];
    Type type;
    int power = GlobalConstants::POWER_MIN_SIZE;
};


Pokemon createPokemon () {

    Pokemon myPokemon;

    std::cout << "Enter the name of your Pokemon : \n";
    std::cin.getline (myPokemon.name, GlobalConstants::NAME_MAX_SIZE);

    char typeStr[GlobalConstants::TYPE_MAX_LENGHT];
    std::cout << "Enter the type of your Pokemon (NORMAL,FIRE,WATER,GRASS,ELECTRIC,GHOST,FLYING) : \n";
    std::cin.getline(typeStr, GlobalConstants::TYPE_MAX_LENGHT);

    if (strcmp(typeStr, "NORMAL") == 0) {
        myPokemon.type = NORMAL;
    } else if (strcmp(typeStr, "FIRE") == 0) {
        myPokemon.type = FIRE;
    } else if (strcmp(typeStr, "WATER") == 0) {
        myPokemon.type = WATER;
    } else if (strcmp(typeStr, "GRASS") == 0) {
        myPokemon.type = GRASS;
    } else if (strcmp(typeStr, "ELECTRIC") == 0) {
        myPokemon.type = ELECTRIC;
    } else if (strcmp(typeStr, "GHOST") == 0) {
        myPokemon.type = GHOST;
    } else if (strcmp(typeStr, "FLYING") == 0) {
        myPokemon.type = FLYING;
    } else {
        std::cout << "Invalid type! \n";
        return {};
    }

    do{
        std::cout << "Enter Pokemon power (between " <<GlobalConstants::POWER_MIN_SIZE<< " and " <<
                  GlobalConstants::POWER_MAX_SIZE << ") :\n";

        std::cin >> myPokemon.power;
    } while (myPokemon.power < GlobalConstants::POWER_MIN_SIZE ||
              myPokemon.power > GlobalConstants::POWER_MAX_SIZE );


    return myPokemon;
}

void writePokemonInBinaryFile (std::ofstream& ofs, const Pokemon& myPokemon) {
    ofs.write( (const char*)&myPokemon, sizeof(Pokemon));
}

Pokemon readPokemonFromBinaryFile (std::ifstream ifs) {

    Pokemon myPokemon;
    ifs.read((char*)&myPokemon, sizeof(Pokemon));

    return myPokemon;
}

struct PokemonHandler {
    const char* fileName;
    int fileSize = 0 ;
    std::fstream fs;
};

int getFileSize (std::ifstream& file) {

    int current = file.tellg();

    file.seekg(0, std::ios::end);
    int fileSize = file.tellg();

    file.seekg(current);
    return fileSize;
}

PokemonHandler newPokemonHandler (const char* fileName) {

    std::ifstream file (fileName, std::ios::binary);
    if(!file.is_open()) {
        return {};
    }
    int fileSize = getFileSize(file);

    PokemonHandler myPokemonHandler;

    myPokemonHandler.fileName = fileName;
    myPokemonHandler.fileSize = fileSize;

    return myPokemonHandler;
}

int size(const PokemonHandler &ph) {

    std::ifstream ifs(ph.fileName, std::ios::binary);
    if(!ifs.is_open()){
        return -1;
    }

    ifs.seekg(0, std::ios::end);

    int size = ifs.tellg() / sizeof(Pokemon);

    ph.fileSize = size;

    return size;

}

Pokemon at(const PokemonHandler& ph, int i) {
    std::ifstream ifs(ph.fileName, std::ios::binary);
    if(!ifs.is_open() || i < 0 || i >= ph.fileSize ){
        return {};
    }

    ifs.seekg( i* sizeof(Pokemon), std::ios::beg);
    Pokemon myPokemon;

    ifs.read((char*)&myPokemon, sizeof(Pokemon));
    return myPokemon;
}



int main() {
    Pokemon myPokemon = createPokemon();
}
