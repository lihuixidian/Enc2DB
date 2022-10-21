#ifndef SYMCIPHER_H
#define SYMCIPHER_H
#include <stdio.h>
#include <stdlib.h>
// #include <memory>

#include <map>
#include <unordered_map>
#include <vector>
#include <string.h>
#include "MathUtils.h"

enum CipherType
{
    ARRAY
};

using namespace std;
class SymCipher
{

protected:
    long long value;
    //************************************************
    //    int sizePos=0;
    //    int sizeNeg=0;
    //
    //    long long offsetPos=0;
    //    long long offsetNeg=0;
    //
    //    // hold the ids
    //
    //    //int *idsPos;
    //    vector<int> idsPos;
    //    //int *idsNeg;
    //    vector<int> idsNeg;
    //
    //
    //    // for all cardinalities not in the map
    //    long long cardMultiplierPos=0;
    //    long long cardMultiplierNeg=0;
    //
    //    // card multiplier does not apply for this map. The map holds the actual cardinality.
    //    // index of "ids" array--> cardinality
    //    unordered_map<int, long long> cardPos;
    //    unordered_map<int, long long> cardNeg;
    //************************************************
    MathUtils math;
    CipherType type;
    // public:
    // CipherType cipher;
    // const CipherType =ARRAY;

public:
    virtual long long getValue();
    const long long &getConstValue();
    void setValue(long long v);
    //*****************************************
    virtual int getsizePos() = 0;
    virtual long long getoffsetPos() = 0;
    virtual long long getcardMultiplierPos() = 0;
    virtual int getsizeNeg() = 0;
    virtual long long getoffsetNeg() = 0;
    virtual long long getcardMultiplierNeg() = 0;
    virtual vector<int> &getidsPos() = 0;
    virtual vector<int> &getidsNeg() = 0;
    virtual unordered_map<int, long long> getcardPos() = 0;
    virtual unordered_map<int, long long> getcardNeg() = 0;
    //***********************************************

    void addValue(long long v, long long modulo);

    void subValue(long long v, long long modulo);

    void multiplyValue(long long v, long long modulo);
    void raiseValue(long long v, long long modulo);

    virtual int getSize() = 0;

    virtual vector<vector<long long>> getIds() = 0;

    virtual void add(SymCipher &other, long long modulo) = 0;

    virtual void sub(SymCipher &other, long long modulo) = 0;

    virtual void multiply(long long m, long long modulo) = 0;

    virtual void multiply(SymCipher &other, long long modulo) = 0;

    virtual void pow(long long m, long long modulo) = 0;

    virtual ~SymCipher() = default;
};
#endif