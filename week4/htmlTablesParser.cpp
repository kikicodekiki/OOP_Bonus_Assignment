#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>


namespace GlobalConstants {
    constexpr size_t CELL_MAX_SIZE = 50; // max 50 symbols
    constexpr size_t ROW_MAX_SIZE = 30; // max 30 cells
    constexpr size_t TABLE_MAX_SIZE = 300; // max 30 rows
    constexpr char SEP = '/';
    constexpr int BUFFER_SIZE = 1024;

}
typedef char Table[GlobalConstants::TABLE_MAX_SIZE];
typedef Table Row[GlobalConstants::ROW_MAX_SIZE];

struct CsvTable {
    Row rows [GlobalConstants::ROW_MAX_SIZE];
    size_t rowCount = 0;
    size_t colCount = 0;
};

size_t getRowCount (const char* fileName) {
    std::stringstream myStream (fileName);

    size_t curRowCount = 0; //because of the first row

    char buffer[GlobalConstants::BUFFER_SIZE];

    while (!myStream.eof()) {
       myStream >> buffer;

       if( strcmp(buffer, "<tr>") == 0 ) {
           curRowCount ++;
       }
    }

    return curRowCount;

}

size_t parseRow (const char* row, Row& toReturn) {
    std::stringstream myStream (row);

    size_t currentColumnCount = 0;

    char buffer[GlobalConstants::BUFFER_SIZE];

    while (!myStream.eof()) {
        myStream >> buffer;

        if( strcmp(buffer, "<td>") == 0 ) {
            currentColumnCount++;
        }
    }

    return currentColumnCount;

}


CsvTable parseFromFile ( std::ifstream& ifs ) {

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


typedef char Table [GlobalConstants::TABLE_MAX_SIZE];
typedef Table Row[GlobalConstants::CELL_MAX_SIZE];

struct CvsTable {
public:
    Row rows[GlobalConstants::ROW_MAX_SIZE];
    size_t rowsCount = 0;
    size_t colsCount = 0;
};

size_t parseNumbers () {

};

size_t parseFromHTML (const char* myStr, Row& toReturn) {

    size_t lengthStr = strlen(myStr);
    size_t begin = 0, end = 0;

    for (size_t i = 0; i < lengthStr; i++) {

        if ( myStr[i] == '>' ) {
            begin = i + 1;
            break;
        }
    }

    while (myStr[begin] == ' ') {
        begin++;
    }

    for ( size_t i = begin; i < lengthStr; i++) {

        if ( myStr[i] == '<' ) {
            end = i - 1;
            break;
        }
    }


}




int main() {

}

