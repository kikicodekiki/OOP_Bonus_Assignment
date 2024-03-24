#include <iostream>
#include <fstream>
#include <cstring>

namespace GlobalConstants {
    constexpr size_t CELL_MAX_SIZE = 50;
    constexpr size_t ROW_MAX_SIZE = 15;
    constexpr size_t TABLE_MAX_SIZE = 100;
    constexpr char SEP = '/';
    constexpr int BUFFER_SIZE = 1024;
    constexpr size_t MAX_FILE_NAME_SIZE = 30;
}

typedef char Cell[GlobalConstants::CELL_MAX_SIZE];
typedef Cell Row[GlobalConstants::ROW_MAX_SIZE];

int isSubstring(const char* tagFromFile, const char* myTag) {
    if (!tagFromFile || !myTag) {
        return -1;
    }

    size_t tagFromFileSize = strlen(tagFromFile);
    size_t myTagSize = strlen(myTag);

    for (int i = 0; i <= tagFromFileSize - myTagSize; i++) {
        for (int j = 0; j < myTagSize; j++) {
            if (myTag[j] != tagFromFile[i + j]) {
                break;
            }

            if (j == myTagSize - 1) {
                return i;
            }
        }
    }

    return -1;
}

int convertCharToDigit(char ch) {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    return -1;
}

unsigned convertStrToUnsigned(const char* str) {
    if (!str)
        return 0;

    unsigned result = 0;
    while (*str) {
        if (*str >= '0' && *str <= '9') {
            int digit = convertCharToDigit(*str);
            if (digit == -1)
                return 0;
            (result *= 10) += digit;
        }
        str++;
    }
    return result;
}

bool isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

class HtmlTable {
private:
    Row rows[GlobalConstants::TABLE_MAX_SIZE] = {};
    size_t rowCount = 0;
    size_t colCount = 0;
    char fileName[GlobalConstants::MAX_FILE_NAME_SIZE] = {};

    void fixAsciiCharacterEntityReference(char* str) {
        if (!str) {
            return;
        }

        char* ptr = str;
        while (*ptr) {
            if (*ptr == '&' && *(ptr + 1) == '#' && *(ptr + 2) != '\0') {
                int asciiCode = 0;
                ptr += 2;
                while (isDigit(*ptr)) {
                    asciiCode = asciiCode * 10 + (*ptr - '0');
                    ptr++;
                }
                *str++ = static_cast<char>(asciiCode);
            } else {
                *str++ = *ptr++;
            }
        }
        *str = '\0';
    }

public:
    HtmlTable() = default;

    void setFileName(const char* newFileName);
    void loadFromFile();
    size_t getRowCount() const;
    void printTable() const;
    void addRow(size_t rowNumber, const char* values[]);
    void removeRow(size_t rowNumber);
    void edit(size_t rowNumber, size_t colNumber, const char* newValue);
};

void HtmlTable::setFileName(const char* newFileName) {
    if (!newFileName || strlen(newFileName) > GlobalConstants::MAX_FILE_NAME_SIZE - 1) {
        std::cerr << "Invalid file name";
        return;
    }

    strcpy(fileName, newFileName);
}

void HtmlTable::loadFromFile() {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error opening file " << fileName;
        return;
    }

    char buff[GlobalConstants::BUFFER_SIZE];
    while (!file.eof()) {
        file.getline(buff, GlobalConstants::BUFFER_SIZE, '<');

        char tagBuff[GlobalConstants::BUFFER_SIZE];
        file.getline(tagBuff, GlobalConstants::BUFFER_SIZE, '>');

        if (isSubstring(tagBuff, "table") != -1) {
            continue;
        } else if (isSubstring(tagBuff, "tr") != -1) {
            continue;
        } else if (isSubstring(tagBuff, "th") != -1 || isSubstring(tagBuff, "td") != -1) {
            file.getline(buff, GlobalConstants::BUFFER_SIZE, '<');
            fixAsciiCharacterEntityReference(buff);

            if (rowCount < GlobalConstants::TABLE_MAX_SIZE && colCount < GlobalConstants::ROW_MAX_SIZE) {
                strcpy(rows[rowCount][colCount], buff);
                colCount++;
            }
        }
    }

    file.close();
}

size_t HtmlTable::getRowCount() const {
    return rowCount;
}

void HtmlTable::addRow(size_t rowNumber, const char* values[]) {
    if (rowNumber > rowCount + 1) {
        std::cerr << "Row number out of bounds\n";
        return;
    }

    if (rowCount >= GlobalConstants::TABLE_MAX_SIZE) {
        std::cerr << "Maximum row count reached\n";
        return;
    }

    for (size_t i = rowCount; i >= rowNumber; --i) {
        for (size_t j = 0; j < colCount; ++j) {
            strcpy(rows[i + 1][j], rows[i][j]);
        }
    }

    for (size_t j = 0; j < colCount; ++j) {
        strcpy(rows[rowNumber][j], values[j]);
    }

    rowCount++;
}

void HtmlTable::removeRow(size_t rowNumber) {
    if (rowNumber > rowCount) {
        std::cerr << "Row number out of bounds\n";
        return;
    }

    for (size_t i = rowNumber - 1; i < rowCount - 1; ++i) {
        for (size_t j = 0; j < colCount; ++j) {
            strcpy(rows[i][j], rows[i + 1][j]);
        }
    }

    rowCount--;
}

void HtmlTable::edit(size_t rowNumber, size_t colNumber, const char *newValue) {
    if (rowNumber > rowCount || colNumber > colCount) {
        std::cerr << "Row or column number out of bounds\n";
        return;
    }

    strcpy(rows[rowNumber - 1][colNumber - 1], newValue);
}

void HtmlTable::printTable() const {
    for (size_t i = 0; i < rowCount; ++i) {
        for (size_t j = 0; j < colCount; ++j) {
            std::cout << "|" << rows[i][j];
        }
        std::cout << "|\n";
    }
}

int main() {

    std::fstream file ("sample.dat", std::ios::in|std::ios::out);

    HtmlTable table;
    table.setFileName("sample.dat");
    table.loadFromFile();

    // Sample manipulation commands
    table.printTable();
    table.addRow(2, (const char*[]){"New", "Row"});
    table.printTable();
    table.removeRow(2);
    table.printTable();
    table.edit(1, 1, "Edited");
    table.printTable();

    return 0;
}

