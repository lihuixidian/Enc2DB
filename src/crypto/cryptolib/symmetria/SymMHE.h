

#ifndef SYMMHE_H
#define SYMMHE_H
#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include <map>
#include <vector>
#include <string.h>
#include "Symcipher.h"
#include "MathUtils.h"
#include "SymPHE.h"
#include "SymAHE.h"

using namespace std;

class SymMHE : public SymPHE
{

private:
    // const string DEFAULT_KEY_PATH = "/tmp/symahe.sk";
    static const long long DEFAULT_MODULO = 9222730058745388403L;
    static const long long DEFAULT_GENERATOR = 6980122786781000881L;
    string key_path_SMHE = "/home/tianqi/openGauss_program/SMHE_key.txt";

public:
    long long g;
    long long modulo = DEFAULT_MODULO;
    // long long negThreshold;
    MathUtils math;
    // g as a BigInteger
    //*********************************
    // public BigInteger gBI;
    //*********************************

public:
    SymMHE();

    SymMHE(CipherType type);
    SymMHE(int negDivisor);

    SymMHE(string path);

    SymMHE(CipherType type, int negDivisor);
    // ArraySymCipher arr;

private:
    void generateModulo();

    //@Override
public:
    void keyGen();

    //@Override
    char *encrypt(long long m);

    //@Override
    long long decrypt(char *);
    // void toString(void);

    SymCipher &multiply(SymCipher &c1, SymCipher &c2);

    SymCipher &multiplyPlaintext(SymCipher &c, long long m);

    SymCipher &divide(SymCipher &c1, SymCipher &c2);

    SymCipher &pow(SymCipher &c, long long m);

    SymCipher &inverse(SymCipher &c);
    void setupNegative(int negDivisor);
    long long handleNegative(long long m);

    //@Override
    // public String toString() {
    //     return "<" + this.getClass().getSimpleName() + ">";
    // }
};
#endif
