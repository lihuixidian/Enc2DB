#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <memory.h>
#include <iostream>
=======
#include <memory>

>>>>>>> dbdev
#include <map>
#include <vector>
#include "Symcipher.h"
#include "MathUtils.h"
#include "ArraySymCipher.h"
#include "SymPHE.h"
#include "SymMHE.h"
#include "t.h"
#include "str22struct.h"
<<<<<<< HEAD
#include <cstring>

#include <ext/hash_map>
using namespace __gnu_cxx;
=======
#include <string.h>
>>>>>>> dbdev

using namespace std;

SymMHE::SymMHE() : SymMHE(ARRAY, DEFAULT_NEGDIVISOR)
{
    // new (this) SymMHE(ARRAY, DEFAULT_NEGDIVISOR);
}
// SymMHE::~SymMHE()
// {
//     delete new (this) SymMHE(ARRAY, DEFAULT_NEGDIVISOR);
// }

SymMHE::SymMHE(CipherType type) : SymMHE(type, DEFAULT_NEGDIVISOR)
{
    // new (this) SymMHE(type, DEFAULT_NEGDIVISOR);
}
// SymMHE::~SymMHE(CipherType type)
// {
//     delete new (this) SymMHE(type, DEFAULT_NEGDIVISOR);
// }

<<<<<<< HEAD
SymMHE::SymMHE(int negDivisor) : SymMHE(ARRAY, negDivisor) 
=======
SymMHE::SymMHE(int negDivisor) : SymMHE(ARRAY, negDivisor)
>>>>>>> dbdev
{
    // new (this) SymMHE(ARRAY, negDivisor);
}
// SymMHE::~SymMHE(int negDivisor)
// {
//     delete new (this) SymMHE(ARRAY, negDivisor);
// }

// SymMHE::SymMHE(string path) {
//    new(this)SymMHE(ARRAY, DEFAULT_NEGDIVISOR);
//}

SymMHE::SymMHE(CipherType type, int negDivisor) : SymPHE(type, negDivisor)
{

    // generateModulo();
    modulo = DEFAULT_MODULO;
    // moduloBI = BigInteger.valueOf(modulo);
    g = DEFAULT_GENERATOR;
    // gBI = BigInteger.valueOf(g);
    setupNegative(negDivisor);
}

// void generateModulo() {
//     moduloBI = BigInteger.probablePrime(long long.SIZE - 1, CryptoScheme.RNG);
//     modulo = moduloBI.long longValue();
//     do {
//         gBI = BigInteger.probablePrime(long long.SIZE - 1, CryptoScheme.RNG);
//     } while (gBI.compareTo(moduloBI) >= 0 || !gBI.gcd(moduloBI).equals(BigInteger.ONE));
//     g = gBI.long longValue();
// }

//@Override
// public void keyGen() {
//     String key = new BigInteger(128, CryptoScheme.RNG).toString(32);
//     FileUtils.saveObjectToFile(key, privateKeyPath);
// }
void SymMHE::setupNegative(int negDivisor)
{
    negThreshold = 0;
    if (negDivisor != 1)
        negThreshold = modulo / negDivisor;
}
long long SymMHE::handleNegative(long long m)
{
    if (negThreshold != 0 && m >= negThreshold)
        m = m - modulo;
    return m;
}
//@Override

<<<<<<< HEAD
char * SymMHE:: encrypt(long long m) {
     long long nextId = getNextId();

     long long a= RandNum(nextId);
     long long obf = math.modPow(g, math.mod(a, modulo), modulo);
     long long v = math.modMul(m, obf, modulo);

     SymCipher * cipher=SymMHE::generateCipher(cipherType, v, nextId);
     t change;
     str22struct str22;
     char *str = NULL;
     string res;
     string structTostr;
     str22struct:: ArraySymCipher_change p ;
//
     p.value=cipher->getValue();
     p.sizePos=cipher->getsizePos();
     p.offsetPos=cipher->getoffsetPos();
     p.cardMultiplierPos=cipher->getcardMultiplierPos();
     p.sizeNeg=cipher->getsizeNeg();
     p.offsetNeg=cipher->getoffsetNeg();
     p.cardMultiplierNeg=cipher->getcardMultiplierNeg();
     p.idsPos.assign(cipher->getidsPos().begin(),cipher->getidsPos().end());
     p.idsNeg.assign(cipher->getidsNeg().begin(),cipher->getidsNeg().end());
     p.cardPos=cipher->getcardPos();
     p.cardNeg=cipher->getcardNeg();
//
     string mapTostrPos = str22.mapToString(p.cardPos);
     string mapTostrNeg = str22.mapToString(p.cardNeg);
//
     res =str22.ArraySymCipher_to_str(&str, &p);
     structTostr=res+mapTostrPos+"%"+mapTostrNeg+"^";
//        //char *ch=(char*)structTostr.data();
//

     size_t bufsz1 = sizeof(structTostr) + 1;
     size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
     char *ret = new char[BUF_SIZE]();
     strcpy(ret, structTostr.data());
     delete cipher;
     return ret;
=======
char *SymMHE::encrypt(long long m)
{
    long long nextId = getNextId();

    long long a = RandNum(nextId);
    long long obf = math.modPow(g, math.mod(a, modulo), modulo);
    long long v = math.modMul(m, obf, modulo);

    SymCipher *cipher = SymMHE::generateCipher(cipherType, v, nextId);
    t change;
    str22struct str22;
    char *str = NULL;
    string res;
    string structTostr;
    str22struct::ArraySymCipher_change p;
    //
    p.value = cipher->getValue();
    p.sizePos = cipher->getsizePos();
    p.offsetPos = cipher->getoffsetPos();
    p.cardMultiplierPos = cipher->getcardMultiplierPos();
    p.sizeNeg = cipher->getsizeNeg();
    p.offsetNeg = cipher->getoffsetNeg();
    p.cardMultiplierNeg = cipher->getcardMultiplierNeg();
    p.idsPos.assign(cipher->getidsPos().begin(), cipher->getidsPos().end());
    p.idsNeg.assign(cipher->getidsNeg().begin(), cipher->getidsNeg().end());
    p.cardPos = cipher->getcardPos();
    p.cardNeg = cipher->getcardNeg();
    //
    string mapTostrPos = str22.mapToString(p.cardPos);
    string mapTostrNeg = str22.mapToString(p.cardNeg);
    //
    res = str22.ArraySymCipher_to_str(&str, &p);
    structTostr = res + mapTostrPos + "%" + mapTostrNeg + "^";
    //        //char *ch=(char*)structTostr.data();
    //
    


    size_t bufsz1 = sizeof(structTostr) + 1;
    size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    char *ret = new char[BUF_SIZE]();
#ifdef ENCLAVE
    strcpy_s(ret, BUF_SIZE, structTostr.data());
#else
    strcpy(ret, structTostr.data());
#endif
    delete cipher;
    return ret;
>>>>>>> dbdev

    //  size_t bufsz1 = sizeof(structTostr) + 1;
    //  size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    //  char* h = new char[hexsz1 * 2];
    //  change.buf2hex((uint8_t*)structTostr.data(), strlen(structTostr.data()), h, &hexsz1);
    //  delete cipher;
    //  return h;
<<<<<<< HEAD

 }
=======
}
>>>>>>> dbdev
// SymCipher& SymMHE:: encrypt(long long m) {
//    long long nextId = getNextId();
//
//    long long a= RandNum(nextId);
//    long long obf = math.modPow(g, math.mod(a, modulo), modulo);
//    long long v = math.modMul(m, obf, modulo);
//    return SymMHE::generateCipher(cipherType, v, nextId);
//}

// //@Override
<<<<<<< HEAD
long long SymMHE::decrypt(char* c)
{
    t change;
    str22struct str22;
    size_t bufsz2 = sizeof(c) + 1;
    size_t hexsz2 = 2 * (strlen(c) + 1);
    char* nc = new char[hexsz2];
    change.hex2buf((uint8_t*)nc, &hexsz2, c, hexsz2);
    char* str = NULL;
    string res;
    string structTostr;

    SymCipher* a = new ArraySymCipher(nc);

    long long m = a->getValue();
    vector<vector<long long>> ids = a->getIds();
    for (int i = 0; i < a->getSize(); i++) {
=======
long long SymMHE::decrypt(char *nc)
{
    // t change;
    str22struct str22;
    // size_t bufsz2 = sizeof(c) + 1;
    // size_t hexsz2 = 2 * (strlen(c) + 1);
    // char *nc = new char[hexsz2];
    // change.hex2buf((uint8_t *)nc, &hexsz2, c, hexsz2);
    char *str = NULL;
    string res;
    string structTostr;

    SymCipher *a = new ArraySymCipher(nc);

    long long m = a->getValue();
    vector<vector<long long>> ids = a->getIds();
    for (int i = 0; i < a->getSize(); i++)
    {
>>>>>>> dbdev
        long long a = math.mod(RandNum(ids[0][i]), modulo);
        long long obf = math.modPow(g, a, modulo);
        long long card = ids[1][i];
        if (card >= 0)
            obf = math.modInverse(obf, modulo);
        else
            card = math.modNegate(card, modulo);
        if (card != 1)
            obf = math.modPow(obf, card, modulo);
        m = math.modMul(m, obf, modulo);
    }
    delete a;
<<<<<<< HEAD
    delete []nc;
=======
    // delete[] nc;
>>>>>>> dbdev
    return handleNegative(m);
}
// long long SymMHE::decrypt(SymCipher& c) {
//    long long m = c.getValue();
//    vector <vector <long long >>ids= c.getIds();
//    for (int i = 0; i < c.getSize(); i++) {
//        long long a=math.mod(RandNum(ids[0][i]), modulo);
//        long long obf = math.modPow(g, a, modulo);
//        long long card = ids[1][i];
//        if (card >= 0)
//            obf = math.modInverse(obf, modulo);
//        else
//            card = math.modNegate(card, modulo);
//        if (card != 1)
//            obf = math.modPow(obf, card, modulo);
//        m = math.modMul(m, obf, modulo);
//    }
//    return handleNegative(m);
//}

// void SymMHE::toString(void){//自己定义的toString()函数
//    printf("ArraySymCipher的value: %lld,sizepos: %d, sizeneg: %d,offsetPos: %ld,offsetNeg: %ld,\n",
//    value,sizePos,sizeNeg,offsetPos,offsetNeg);
//}

<<<<<<< HEAD
SymCipher& SymMHE::multiply(SymCipher& c1, SymCipher& c2)
=======
SymCipher &SymMHE::multiply(SymCipher &c1, SymCipher &c2)
>>>>>>> dbdev
{
    c1.multiply(c2, modulo);
    return c1;
}

<<<<<<< HEAD
SymCipher& SymMHE::multiplyPlaintext(SymCipher& c, long long m)
=======
SymCipher &SymMHE::multiplyPlaintext(SymCipher &c, long long m)
>>>>>>> dbdev
{
    c.multiplyValue(m, modulo);
    return c;
}

<<<<<<< HEAD
SymCipher& SymMHE::pow(SymCipher& c, long long m)
=======
SymCipher &SymMHE::pow(SymCipher &c, long long m)
>>>>>>> dbdev
{
    c.pow(m, modulo);
    return c;
}
<<<<<<< HEAD
SymCipher& SymMHE::inverse(SymCipher& c)
{
    return pow(c, -1L);
}
SymCipher& SymMHE::divide(SymCipher& c1, SymCipher& c2)
=======
SymCipher &SymMHE::inverse(SymCipher &c)
{
    return pow(c, -1L);
}
SymCipher &SymMHE::divide(SymCipher &c1, SymCipher &c2)
>>>>>>> dbdev
{
    return multiply(c1, inverse(c2));
}

//@Override
// String toString() {
//     return "<" + this.getClass().getSimpleName() + ">";
// }
