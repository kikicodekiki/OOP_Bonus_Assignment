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

Pokemon readPokemonFromBinaryFile (std::ifstream& ifs) {

    Pokemon myPokemon;
    ifs.read((char*)&myPokemon, sizeof(Pokemon));

    return myPokemon;
}

struct PokemonHandler {
    const char* fileName;
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

    return myPokemonHandler;
}

int size(const PokemonHandler &ph) {

    std::ifstream ifs(ph.fileName, std::ios::binary);
    if(!ifs.is_open()){
        return -1;
    }

    ifs.seekg(0, std::ios::end);

    int size = ifs.tellg() / sizeof(Pokemon);

    return size;

}

bool notValid (const PokemonHandler& ph, int i) {
    return (i < 0 || i > size(ph));
}

Pokemon at(const PokemonHandler& ph, int i) {
    std::ifstream ifs(ph.fileName, std::ios::binary);
    if(!ifs.is_open() || notValid (ph, i) ){
        return {};
    }

    ifs.seekg( i* sizeof(Pokemon), std::ios::beg);
    Pokemon myPokemon;

    ifs.read((char*)&myPokemon, sizeof(Pokemon));
    return myPokemon;
}

void swap (const PokemonHandler& ph, int i, int j) {
    std::ofstream ofs (ph.fileName, std::ios::binary);

    if(!ofs.is_open() || notValid(ph, i) || notValid(ph,j) || i==j ) {
        return;
    }

    Pokemon iPokemon = at(ph, i);
    Pokemon jPokemon = at(ph, j);

    ofs.seekp(i*sizeof(Pokemon), std::ios::beg);
    ofs.write((const char*)& jPokemon, sizeof(Pokemon));

    ofs.seekp(j*sizeof(Pokemon), std::ios::beg);
    ofs.write((const char*)& iPokemon, sizeof(Pokemon));

    ofs.close();

}

void insertionSort(const PokemonHandler& ph) {
    std::fstream file(ph.fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!file.is_open()) {
        return;
    }

    int n = size(ph);
    if (n <= 1) {
        return;
    }

    for (int i = 1; i < n; ++i) {
        Pokemon key = at(ph, i);
        int j = i - 1;
        while (j >= 0 && at(ph, j).power > key.power) {
            swap(ph, j + 1, j);
            --j;
        }

        file.write((const char*)&key, sizeof(Pokemon));
    }
}

void insert(const PokemonHandler &ph, const Pokemon &pokemon) {

    std::ofstream ofs (ph.fileName, std::ios::binary | std::ios::app);
    if(ofs.is_open()) {
        return;
    }


    writePokemonInBinaryFile(ofs, pokemon);
    insertionSort(ph);

    ofs.close();
}

void textify(const PokemonHandler &ph, const char* fileName) {

    std::ifstream ifs (ph.fileName, std::ios::binary | std::ios::in);
    if(!ifs.is_open())
        return;

    std::ofstream ofs(fileName);
    if(ofs.is_open())
        return;

    while(!ifs.eof()) {

        Pokemon myPokemon = readPokemonFromBinaryFile(ifs);
        writePokemonInBinaryFile(ofs,myPokemon);
        std::cout << std::endl;
    }

    ifs.close();
    ofs.close();
}

void untextify(const PokemonHandler &ph, const char* fileName) {

    std::ifstream ifs(fileName);
    if (!ifs.is_open()) {
        return;
    }

    std::ofstream ofs(ph.fileName, std::ios::binary | std::ios::trunc);
    if (!ofs.is_open()) {
        ifs.close();
        return;
    }

    while (!ifs.eof()) {
        Pokemon myPokemon = readPokemonFromBinaryFile(ifs);
        if(!ifs.eof()){
            break;
        }
        writePokemonInBinaryFile(ofs, myPokemon);
    }

    ifs.close();
    ofs.close();

}


int main() {
    PokemonHandler handler = newPokemonHandler("ph.dat");

    Pokemon p1 = { "Pikachu", ELECTRIC, 250 };
    Pokemon p2 = { "Charmander", FIRE, 300 };
    Pokemon p3 = { "Squirtle", WATER, 280 };

    insert(handler, p1);
    insert(handler, p2);
    insert(handler, p3);

    textify(handler, "pokemon_data.txt");
    untextify(handler, "pokemon_data.txt");

    return 0;
}
