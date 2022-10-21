#ifndef SYMAHE_H
#define SYMAHE_H
#include <memory>

#include <map>
#include <vector>
#include <string.h>
#include "Symcipher.h"
#include "MathUtils.h"
#include "ArraySymCipher.h"
#include "CryptoScheme.h"
#include "SymPHE.h"
#include <limits.h>

using namespace std;

// static const long long modulo = LLONG_MAX;
class SymAHE : public SymPHE
{

private:
    // const string DEFAULT_KEY_PATH = "/home/cp/tql/SAHE_key.txt";
    // string key_path_SAHE = "/home/tianqi/openGauss_program/SAHE_key.txt";

public:
    SymAHE();
    long long modulo = LLONG_MAX;
    // long long negThreshold;
    MathUtils math;

    explicit SymAHE(CipherType type);

    explicit SymAHE(int negDivisor);
    // explicit SymAHE(string path);

    SymAHE(CipherType type, int negDivisor);

    // @Override
    //    void keyGen();

    // @Override
    char *encrypt(long long m);

    // SymCipher& encrypt(long long m);

    long long decrypt(char *c);

    SymCipher &add(SymCipher &c1, SymCipher &c2);

    SymCipher &addPlaintext(SymCipher &c, long long m);

    SymCipher &subtract(SymCipher &c1, SymCipher &c2);

    SymCipher &multiply(SymCipher &c, long long m);

    SymCipher &neggate(SymCipher &c);

    void setupNegative(int negDivisor);
    long long handleNegative(long long m);

    // String toString();
};

#endif