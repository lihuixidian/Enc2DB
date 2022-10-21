#ifndef ARRAYSYMCIPHER_H
#define ARRAYSYMCIPHER_H
#include <memory>

#include <map>
#include <unordered_map>
#include <vector>
#include <string.h>
#include "Symcipher.h"
#include "MathUtils.h"

#include "str22struct.h"

using namespace std;

class ArraySymCipher : public SymCipher
{

    MathUtils math;

private:
    //新增在类里定义value值
    //    long long value;
    int sizePos = 0;
    int sizeNeg = 0;

    long long offsetPos = 0;
    long long offsetNeg = 0;

    // hold the ids

    // int *idsPos;
    vector<int> idsPos;
    // int *idsNeg;
    vector<int> idsNeg;

    // for all cardinalities not in the map
    long long cardMultiplierPos = 0;
    long long cardMultiplierNeg = 0;

    // card multiplier does not apply for this map. The map holds the actual cardinality.
    // index of "ids" array--> cardinality
    unordered_map<int, long long> cardPos;
    unordered_map<int, long long> cardNeg;

    //新增的代码
public:
    str22struct str;
    ArraySymCipher(string cipher);
    long long getValue();
    int getsizePos();
    long long getoffsetPos();
    long long getcardMultiplierPos();
    int getsizeNeg();
    long long getoffsetNeg();
    long long getcardMultiplierNeg();
    vector<int> &getidsPos();
    vector<int> &getidsNeg();
    unordered_map<int, long long> getcardPos();
    unordered_map<int, long long> getcardNeg();

public:
    ArraySymCipher(long long value, long long id);

private:
    void addIds(ArraySymCipher other, bool isPos);

private:
    void addIds(ArraySymCipher other);

private:
    void multiplyIds(long long multiplier, long long modulo);

    // sizePos + sizeNeg
    /**
     * Return the number of ids currently stored.
     */
public:
    int getSize();

    //
    //@Override
public:
    vector<vector<long long>> getIds();

    void add(SymCipher &other, long long modulo) override;

    void sub(SymCipher &other, long long modulo) override;

    void multiply(long long m, long long modulo) override;

    void multiply(SymCipher &other, long long modulo) override;

    void pow(long long m, long long modulo) override;
};

#endif