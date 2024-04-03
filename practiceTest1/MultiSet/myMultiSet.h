#pragma once

#include <iostream>

class MultiSetLessThanThree {
private:
    uint8_t *buckets = nullptr; //dynamic memory -> BIG 4
    unsigned countOfBuckets = 0;
    unsigned Num = 0;
    const unsigned elementsInBucket = 4;
    const unsigned maxOccurrences = 3;

    void free();
    void copyFrom();

public:

    //default constructor
    MultiSetLessThanThree() = default;
    explicit MultiSetLessThanThree(unsigned newNum);
    //copy constructor
    MultiSetLessThanThree(const MultiSetLessThanThree& other);
    //operator=
    MultiSetLessThanThree& operator= (const MultiSetLessThanThree& other);
    //destructor
    ~MultiSetLessThanThree();

    bool add (unsigned n);
    bool remove (unsigned n);
    bool occurrences (unsigned n);

private:

    void printNum (unsigned num, unsigned occurrences) const;
    void addNumMultipleTimes (unsigned num, unsigned times);

public:

    void printSet ();

    friend MultiSetLessThanThree intersect (const MultiSetLessThanThree& lhs, const MultiSetLessThanThree& rhs);
    friend MultiSetLessThanThree unify (const MultiSetLessThanThree& lhs, const MultiSetLessThanThree& rhs);
};
