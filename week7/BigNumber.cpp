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

void BigNumber::copyFrom(const BigNumber &other) {
    number = new char[other.size + 1];
    size = other.size + 1;
    strcpy(number, other.number);
    isPositiveNumber = other.isPositiveNumber;
}

BigNumber::BigNumber (const BigNumber& other){
    copyFrom(other);
}

BigNumber& BigNumber::operator=(const BigNumber &other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

bool operator == (const BigNumber& lhs, const BigNumber& rhs)  {
    if (lhs.isPositiveNumber != rhs.isPositiveNumber) {
        return false;
    }

    if(lhs.size != rhs.size) {
        return false;
    }

    for (size_t i = 0; i < lhs.size; ++i) {
        if (lhs.number[i] != rhs.number[i]) {
            return false;
        }
    }

    return true;
}

bool operator != (const BigNumber& lhs, const BigNumber& rhs) {
    return !(lhs == rhs);
}

bool operator < (const BigNumber& lhs, const BigNumber& rhs) {
    if ((lhs.isPositiveNumber) && !(rhs.isPositiveNumber)) {
        return false;
    } else if (!lhs.isPositiveNumber && rhs.isPositiveNumber) {
        return true;
    }

    if (lhs.size != rhs.size) {
        if (lhs.isPositiveNumber) { //  positive
            return lhs.size < rhs.size; // Less digits
        } else { // negative
            return lhs.size > rhs.size; // Less digits means a larger number
        }
    }

    for (int i = lhs.size - 1; i >= 0; --i) { // Start from the most significant digit
        if (lhs.number[i] < rhs.number[i]) {
            return true;
        } else if (lhs.number[i] > rhs.number[i]) {
            return false;
        }
    }

    return false; // the digits are the same
}

bool operator> (const BigNumber& lhs, const BigNumber& rhs) {
    return !(lhs < rhs) && !(rhs==lhs);
}


