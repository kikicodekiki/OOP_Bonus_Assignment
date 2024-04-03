#include "MultiSet.h"

#include <iostream>

void MultiSetLessThanThree::free() {
    delete[] buckets;
    buckets = nullptr;
    Num = 0;
    countOfBuckets = 0;
}

void MultiSetLessThanThree::copyFrom(const MultiSetLessThanThree& other) {
    Num = other.Num;
    countOfBuckets = other.countOfBuckets;

    for (size_t i = 0; i < countOfBuckets; i++) {
        buckets[i] = other.buckets[i];
    }

    buckets[countOfBuckets] = '\0';
}

MultiSetLessThanThree::MultiSetLessThanThree(unsigned newNum) {
    if (newNum > 1000) {
        MultiSetLessThanThree();
    }

    Num = newNum;
    countOfBuckets = Num / elementsInBucket + 1;
    buckets = new uint8_t [countOfBuckets] {0};

}

MultiSetLessThanThree::MultiSetLessThanThree(const MultiSetLessThanThree& other) {
    copyFrom(other);
}

MultiSetLessThanThree& MultiSetLessThanThree::operator=(const MultiSetLessThanThree &other) {

    if(this != &other) {
        free();
        copyFrom(other);
    }

    return *this;
}

MultiSetLessThanThree::~MultiSetLessThanThree() {
    free();
}

bool MultiSetLessThanThree::add (unsigned n) {

    unsigned bucketIndex = getBucketIndex(n);
    unsigned bitIndex = getBitIndex(n);

    uint8_t& currBucket = buckets[bucketIndex]; //za da mozhem direktno da manipulirame

    uint8_t mask = 1 << (bitIndex*2); //zashtoto dve kkletki predstavqt edna cifra

    unsigned occurrences = getCountOccurrences(n);

    switch (occurrences) {
        case 0:
        case 2:
            currBucket |= mask;
            return true;
        case 1:
            currBucket &= ~mask; //setva purviq
            mask <<= 1; //otiva na vtoriq
            currBucket |= mask; //setva i vtoriq
            return true;
        default:
            return false;
    }


}

bool MultiSetLessThanThree::remove (unsigned int n) {

    unsigned bucketIndex = getBucketIndex(n);
    unsigned bitIndex = getBitIndex(n);

    uint8_t& currBucket = buckets[bucketIndex]; //za da mozhem direktno da manipulirame

    uint8_t mask = 1 << (bitIndex*2); //zashtoto dve kkletki predstavqt edna cifra

    mask = ~mask;

    unsigned occurrences = getCountOccurrences(n);

    switch (occurrences) {
        case 3:
        case 1: {
            currBucket &= mask;
            return true;
        }

        case 2 : {
            mask = ~mask;
            currBucket &= mask; //setva purviq
            mask <<= 1;
            currBucket &= ~mask; //setva vtoriq

            return true;

        }

        default: return false;

    }

}

unsigned MultiSetLessThanThree::getBucketIndex(unsigned num) const {
    return Num / elementsInBucket;
}

unsigned MultiSetLessThanThree::getBitIndex (unsigned num) const {
    return Num % elementsInBucket;
}
