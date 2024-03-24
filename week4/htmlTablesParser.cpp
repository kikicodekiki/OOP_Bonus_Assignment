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

bool isDigit (char ch) {
    return ch>='0' && ch <='9';
}

class HtmlTable {
private:
    Row rows [GlobalConstants::TABLE_MAX_SIZE] = {} ;
    size_t rowCount = 0;
    size_t colCount = 0;
    char fileName [GlobalConstants::MAX_FILE_NAME_SIZE] = {};

    void fixAsciiCharacterEntityReference(char* str) {
        if(!str) {
            return;
        }

        char* ptr = str;
        while (*str) {
            if( *(ptr) == '&' && *(ptr+1) == '#' && *(ptr+2) != '\0'){
                int asciiCode = 0;
                ptr += 2; // move past "&#"
                while (isDigit(*ptr)) {
                    asciiCode = asciiCode * 10 + (*ptr - '0');
                    ptr++;
                }
                *str++ = (char)(asciiCode);
                ptr++;
            } else {
                *str++ = *ptr++;
            }

        }
        *str = '\0';
    }
public:
    HtmlTable() = default;

    explicit HtmlTable (const char* newFileName) {
        setColCount();
        setRowCount();
        setFileName(newFileName);
    }

    void setFileName (const char* newFileName);
    void setRowCount ();
    void setColCount ();
    void saveToFile () const;
    void saveToFileNew () const;
    size_t getRowCount () const;
    void loadFromFile();
    void parseRow (const char* row);
    void trunc ();
    void addRow (size_t rowNumber, const char* values[] );
    void removeRow (size_t rowNumber);
    void edit (size_t rowNumber, size_t colNumber, const char* newValue);
    void printTable() const;
};

void HtmlTable::setFileName (const char* newFileName) {
    if (!newFileName || strlen(newFileName) > GlobalConstants::MAX_FILE_NAME_SIZE - 1 ) {
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

    setColCount(); // Update colCount based on the actual number of cells in each row

    char buff[GlobalConstants::BUFFER_SIZE];
    while (!file.eof()) {
        file.getline(buff, GlobalConstants::BUFFER_SIZE, '<');

        char tagBuff[GlobalConstants::BUFFER_SIZE];
        file.getline(tagBuff, GlobalConstants::BUFFER_SIZE, '>');

        if (isSubstring(tagBuff, "table") != -1) {
            // Ignore table tag
            continue;
        } else if (isSubstring(tagBuff, "tr") != -1) {
            // Increment rowCount
            rowCount++;
            colCount = 0; // Reset column count for new row
        } else if (isSubstring(tagBuff, "th") != -1 || isSubstring(tagBuff, "td") != -1) {
            // Process th and td tags
            file.getline(buff, GlobalConstants::BUFFER_SIZE, '<');
            fixAsciiCharacterEntityReference(buff); // Fix ASCII character entity reference

            // Add the cell to the current row
            if (rowCount < GlobalConstants::TABLE_MAX_SIZE && colCount < GlobalConstants::ROW_MAX_SIZE) {
                strcpy(rows[rowCount][colCount], buff);
                colCount++;
            }
        }
    }

    file.close();
}



void HtmlTable::setRowCount() {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error opening " << fileName;
        return;
    }

    char line[GlobalConstants::BUFFER_SIZE];
    rowCount = 0;
    bool inTableRow = false;

    while (file.getline(line, GlobalConstants::BUFFER_SIZE)) {
        if (isSubstring(line, "<tr>") != -1) {
            // start of a new table row
            inTableRow = true;
            rowCount++;
        } else if (isSubstring(line, "</tr>") != -1) {
            // end of the current table row
            inTableRow = false;
        } else if (inTableRow && (isSubstring(line, "<td>") != -1 ||
                    isSubstring(line, "<th>") != -1)) {
            
            rowCount++;
            inTableRow = false;  // to avoid counting multiple cells in one line as multiple rows
        }
    }

    file.close();
}


size_t HtmlTable::getRowCount() const {
    return rowCount;
}

void HtmlTable::parseRow(const char* row) {
    if (rowCount >= GlobalConstants::TABLE_MAX_SIZE){
        return;
    }

    if (colCount < GlobalConstants::ROW_MAX_SIZE) {
        strcpy(rows[rowCount][colCount], row);
        colCount++;
    } else {
        rowCount++;
        colCount = 0;
        strcpy(rows[rowCount][colCount], row);
        colCount++;
    }
}

void HtmlTable::setColCount() {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error opening " << fileName;
        return;
    }

    char buff[GlobalConstants::BUFFER_SIZE];
    size_t maxColCount = 0;

    // Reset file pointer to the beginning of the file
    file.clear();
    file.seekg(0, std::ios::beg);

    while (!file.eof()) {
        file.getline(buff, GlobalConstants::BUFFER_SIZE, '<');
        if (isSubstring(buff, "<tr") != -1) {
            size_t curColCount = 0;
            while (!file.eof() && !isSubstring(buff, "</tr>") && curColCount < GlobalConstants::ROW_MAX_SIZE) {
                file.getline(buff, GlobalConstants::BUFFER_SIZE, '>');
                if (isSubstring(buff, "<th") != -1 || isSubstring(buff, "<td") != -1) {
                    // Check if the content between the tags is empty
                    char content[GlobalConstants::BUFFER_SIZE];
                    file.getline(content, GlobalConstants::BUFFER_SIZE, '<');
                    if (strlen(content) > 0) {
                        curColCount++;
                    }
                }
            }
            maxColCount = std::max(maxColCount, curColCount);
        }
    }

    colCount = maxColCount;

    file.close();
}



void HtmlTable::addRow (size_t rowNumber, const char* values[]) {
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
    if(rowNumber > rowCount) {
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

void HtmlTable::trunc () {
    std::ofstream out (fileName, std::ios::trunc);
    if(!out.is_open()) {
        std::cerr << "Error opening " << fileName;
        return;
    }
    out.close();
}

void HtmlTable::saveToFile () const {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        return;
    }

    file << "<table>" << std::endl;
    for (int i = 0; i < rowCount; i++) {
        file << "  <tr>" << std::endl;
        for (int j = 0; j < colCount; j++) {
            if (isSubstring(rows[i][j], "<th>") != -1) {
                file << "    <th>" << rows[i][j] << "</th>" << std::endl;
            }
            else if (isSubstring(rows[i][j], "<td>") != -1) {
                file << "    <td>" << rows[i][j] << "</td>" << std::endl;
            }
        }
        file << "  </tr>" << std::endl;
    }
    file << "</table>" << std::endl;

    file.close();
}

void HtmlTable::saveToFileNew() const {
    std::ofstream file(fileName, std::ios::app);

    if(!file.is_open()) {
        std::cerr << "Error opening " << fileName;
        return;
    }

    for (size_t i = 0; i < rowCount; ++i) {
        for (size_t j = 0; j < colCount; ++j) {
            file << "|" << rows[i][j];
        }
        file << "|\n";
    }
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

    const char fileName[GlobalConstants::MAX_FILE_NAME_SIZE] = "sample.txt";
    std::fstream file (fileName, std::ios::in | std::ios::out);

    char buff[GlobalConstants::BUFFER_SIZE];
    while (!file.eof()) {
        file.getline(buff, GlobalConstants::BUFFER_SIZE, '>');
        if (isSubstring(buff, "<table") != -1) {
            break;
        }
    }
    HtmlTable table (fileName);
    table.loadFromFile();


    table.printTable();
    table.addRow(2, (const char*[]){"New", "Row"});
    table.printTable();
    table.removeRow(2);
    table.printTable();
    table.edit(1, 1, "Edited");
    table.printTable();

    return 0;

}
