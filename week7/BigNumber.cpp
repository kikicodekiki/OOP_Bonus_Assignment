#include "BigNumber.h"
#include <cmath>
#include <cstring>

bool isNumber(char ch) {
    return (ch >= '0' && ch <= '9');
}

bool containsOnlyNumbers(const char* str) {
    if (str[0] == '-') {
        for (size_t i = 1; str[i] != '\0'; i++) {
            if (!isNumber(str[i]))
                return false;
        }
        return true; // Correctly handles negative numbers.
    }

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isNumber(str[i]))
            return false;
    }
    return true;
}

void BigNumber::setNumber(const char* newNum) {
    if (!newNum || !containsOnlyNumbers(newNum)) {
        return;
    }

    free(); // Ensure we don't leak memory.
    number = new char[strlen(newNum) + 1];
    strcpy(number, newNum);
    size = strlen(newNum);
    isPositiveNumber = (newNum[0] != '-');
}

BigNumber::BigNumber(const char* newNum) {
    setNumber(newNum);
}

void BigNumber::free() {
    delete[] number;
    number = nullptr;
    size = 0;
    isPositiveNumber = true;
}

void fromLongLongIntToStr(char* buff, long long int value) {
    int index = 0;
    bool isNegative = value < 0;
    long long int n = isNegative ? -value : value;

    while (n > 0) {
        buff[index++] = (n % 10) + '0';
        n /= 10;
    }

    if (isNegative) {
        buff[index++] = '-';
    }

    buff[index] = '\0';

    // Reverse the string
    int start = 0;
    int end = index - 1;
    while (start < end) {
        char temp = buff[start];
        buff[start] = buff[end];
        buff[end] = temp;
        start++;
        end--;
    }
}

BigNumber::BigNumber(long long int newNum) {
    if (newNum == 0) {
        number = new char[2];
        number[0] = '0';
        number[1] = '\0';
        size = 1;
        isPositiveNumber = true;
    } else {
        isPositiveNumber = (newNum >= 0);
        size_t arrLen = (size_t)log10(std::abs(newNum)) + 1;
        if (!isPositiveNumber) {
            arrLen++;
        }
        number = new char[arrLen + 1];
        fromLongLongIntToStr(number, newNum);
        size = arrLen;
    }
}
