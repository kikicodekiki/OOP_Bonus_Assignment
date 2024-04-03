#pragma once

#include <iostream>

class BigNumber {
    char* myBigNum = nullptr; //BIG 4

    bool checkOnlyDigits (const char* num);

    void free();
    void copyFrom(const BigNumber& other);

public:
    BigNumber() = default;
    explicit BigNumber(const char* newNum);
    BigNumber(long long int newNum);

    BigNumber& operator= (const BigNumber& other);


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

    ~BigNumber();
};
