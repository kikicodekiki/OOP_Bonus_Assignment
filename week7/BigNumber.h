#pragma once
#include <iostream>
#include <cstring>

class BigNumber {
private:
    char* number = nullptr;
    bool isPositiveNumber = true; //bool flag for sign of the number
    size_t size = 0;
    size_t allocatedDataSize = 0;

    void resize();
    void copyFrom (const BigNumber& other);
    void free();

public:

    BigNumber() = default;
    BigNumber(const char* newNum);
    BigNumber(long long int newNum);

    bool operator == (const BigNumber& other) const;
    bool operator != (const BigNumber& other) const;
    bool operator < (const BigNumber& other) const;
    bool operator > (const BigNumber& other) const;

    BigNumber& operator += (const BigNumber& other);
    BigNumber operator + (const BigNumber& other) const;
    BigNumber& operator -= (const BigNumber& other);
    BigNumber operator - (const BigNumber& other) const;
    BigNumber& operator *= (const BigNumber& other);
    BigNumber operator * (const BigNumber& other) const;

    BigNumber& operator ++ ();
    BigNumber operator ++ (int);

    BigNumber& operator -- ();
    BigNumber operator -- (int);

    BigNumber& operator - ();

    BigNumber& operator /= (const BigNumber& other);
    BigNumber operator / (const BigNumber& other) const;
    BigNumber& operator %= (const BigNumber& other);
    BigNumber operator % (const BigNumber& other) const;

    friend std::ostream& operator<<(std::ostream& out, const BigNumber& number);
    friend std::istream& operator>>(std::istream& in, BigNumber& number);

};