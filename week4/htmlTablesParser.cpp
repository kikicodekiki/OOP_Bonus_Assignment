#include <iostream>
#include <fstream>
#include <cstring>



namespace GlobalConstants {
    constexpr size_t CELL_MAX_SIZE = 50; // max 50 symbols
    constexpr size_t ROW_MAX_SIZE = 15; // max 15 cells
    constexpr size_t TABLE_MAX_SIZE = 100; // max 100 rows
    constexpr char SEP = '/';
    constexpr int BUFFER_SIZE = 1024;
    constexpr size_t MAX_FILE_NAME_SIZE = 30;

}
typedef char Cell[GlobalConstants::CELL_MAX_SIZE];
typedef Cell Row [GlobalConstants::ROW_MAX_SIZE];

int isSubstring (const char* tagFromFile, const char* myTag) {
    if(!tagFromFile || !myTag) {
        return -1;
    }

    size_t tagFromFileSize = strlen(tagFromFile);
    size_t myTagSize = strlen (myTag);

    int i, j;

    for (i = 0; i <= tagFromFileSize - myTagSize; i++) {
        for(j = 0; j < tagFromFileSize; j++) {
            if ( myTag[ i + j ] != tagFromFile[j] ){
                break;
            }

            if (j == tagFromFileSize) {
                return i;
            }
        }
    }

    return -1;
}


bool tagIsPresent (const char* fileName, const char* seekedTag) {
    std::ifstream file (fileName, std::ios::in);

    if ( !file.is_open() ) {
        std::cerr << "Error opening file: " << fileName;
        return -1;
    }

    char buff[1024];
    bool tagIsPresent = false;
    bool slashIsPresent = false;
    bool characterEntityPresent = false;

    while ( !file.eof() ) {
        file.getline(buff, '>');
        if (isSubstring(buff, seekedTag) != -1) {
            tagIsPresent = true;
        }

    }

}

int convertCharToDigit(char ch)
{
    if(ch >= '0' && ch <= '9')
        return ch - '0';
    return -1;
}
unsigned convertStrToUnsigned(const char* str)
{
    if (!str)
        return 0;

    unsigned result = 0;
    unsigned int i = 0;
    while (*str)
    {
        if(str[i] >= '0' && str[i] <= '9') {
            int digit = convertCharToDigit(*str);
            if (digit == -1)
                return 0;
            (result *= 10) += digit;
            str++;
        }
    }
    return result;
}

bool isDigit (char ch) {
    return ch>='0' && ch <='9';
}

class HtmlTable {
private:
    Row rows [GlobalConstants::TABLE_MAX_SIZE] = {} ;
    size_t rowCount = 0;
    size_t colCount = 0;
    char fileName [GlobalConstants::MAX_FILE_NAME_SIZE] = {};
public:
    HtmlTable() = default;

    const Row& getRow () const;
    void setFileName (const char* newFileName);
    size_t parseRow ();
    void setRowCount ();
    void setColCount ();
    void printTable ();
    void saveToFile ();
    size_t getRowCount () const;
    void loadFromFile();
    void parseRow (const char* row, std::ifstream& file);

};

void HtmlTable::setFileName (const char* newFileName) {
    if (!newFileName || strlen(newFileName) > GlobalConstants::MAX_FILE_NAME_SIZE - 1 ) {
        std::cerr << "Invalid file name";
        return;
    }

    strcpy(fileName, newFileName);
}

void HtmlTable::loadFromFile() {
    std::ifstream file (fileName);
    if(!file.is_open()) {
        std::cerr << "Error opening file " << fileName;
        return;
    }

    char buff[GlobalConstants::BUFFER_SIZE];
    while(!file.eof()) {
        file.getline(buff, GlobalConstants::BUFFER_SIZE, '>');

        if(isSubstring(buff, "tr") == -1) {
            parseRow(buff, file);
        }
    }

    file.close();
}

void HtmlTable::setRowCount () {
    std::ifstream file (fileName);

    if( !file.is_open() ) {
        std::cerr << "Error opening " << fileName;
        return ;
    }

    char buff[1024];

    while ( !file.eof() ) {
        file.getline(buff, GlobalConstants::TABLE_MAX_SIZE, '>');
        if (isSubstring(buff, "/tr") != -1) {
            rowCount++;
        }
    }
}

size_t HtmlTable::getRowCount() const {
    return rowCount;
}

void HtmlTable::parseRow(const char* row, std::ifstream& file) {
    size_t currentColumnCount = 0;
    size_t pos = 0;
    char cell[GlobalConstants::CELL_MAX_SIZE];

    while (row[pos] != '\0' && currentColumnCount < GlobalConstants::ROW_MAX_SIZE) {
        while(row[pos] != 't' && row[pos] != 'r')
    }
}

void HtmlTable:: setColCount () {

}


int main() {

}
