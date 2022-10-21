
#include <memory>

#include <map>
#include <utility>
#include <vector>
#include <string.h>
#include "Symcipher.h"
#include "MathUtils.h"
#include "SymPHE.h"
#include "SymAHE.h"
#include "ArraySymCipher.h"
#include "CryptoScheme.h"
#include "t.h"
#include "str22struct.h"

using namespace std;

SymAHE::SymAHE() : SymAHE(ARRAY, DEFAULT_NEGDIVISOR) // 委托构造
{
}
// SymAHE::~SymAHE()
// {
//     delete new (this) SymAHE(ARRAY, DEFAULT_NEGDIVISOR);
// }

SymAHE::SymAHE(CipherType type) : SymAHE(type, DEFAULT_NEGDIVISOR)
{
}
// SymAHE::~SymAHE(CipherType type)
// {
//     delete new (this) SymAHE(type, DEFAULT_NEGDIVISOR);
// }

SymAHE::SymAHE(int negDivisor) : SymAHE(ARRAY, negDivisor)
{
}
// SymAHE::~SymAHE(int negDivisor)
// {
//     delete new (this) SymAHE(ARRAY, negDivisor);
// }

//    SymAHE::SymAHE(string path) {
//        new (this)SymAHE(ARRAY, DEFAULT_NEGDIVISOR, std::move(path));
//    }

SymAHE::SymAHE(CipherType type, int negDivisor) : SymPHE(type, negDivisor)
{
    //        SymPHE(type, negDivisor,std::move(privateKeyPath));
    modulo = LLONG_MAX;
    // moduloBI = BigInteger.valueOf(modulo);
    setupNegative(negDivisor);
}

// @Override
// public void keyGen() {
//     String key = new BigInteger(128, RNG).toString(32);
//     FileUtils.saveObjectToFile(key, privateKeyPath);
// }

void SymAHE::setupNegative(int negDivisor)
{
    negThreshold = 0;
    if (negDivisor != 1)
        negThreshold = modulo / negDivisor;
}
long long SymAHE::handleNegative(long long m)
{
    if (negThreshold != 0 && m >= negThreshold)
        m = m - modulo;
    return m;
}
// @Override
char *SymAHE::encrypt(long long m)
{
    long long nextId = getNextId();
    long long a = RandNum(nextId);
    long long v = math.modAdd(m, math.mod(a, modulo), modulo);
    //SymCipher* cipher;
    SymCipher *cipher = SymAHE::generateCipher(cipherType, v, nextId);

    //*cipher=SymAHE::generateCipher(cipherType, v, nextId);
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

    // size_t bufsz1 = sizeof(structTostr) + 1;
    // size_t hexsz1 = 2 * (strlen(structTostr.data()) + 1);
    // char* h = new char[hexsz1 * 2];
    // change.buf2hex((uint8_t*)structTostr.data(), strlen(structTostr.data()), h, &hexsz1);
    // delete cipher;
    // return h;
}
//********************************************************

long long SymAHE::decrypt(char *nc)
{
    // t change;
    str22struct str22;
    // size_t bufsz2 = sizeof(c) + 1;
    // size_t hexsz2 = 2 * (strlen(c) + 1);
    // char* nc = new char[hexsz2];
    // change.hex2buf((uint8_t*)nc, &hexsz2, c, hexsz2);
    char *str = NULL;
    string res;
    string structTostr;

    SymCipher *a = new ArraySymCipher(nc);
    long long m = a->getValue();
    vector<vector<long long>> ids = a->getIds();
    for (int i = 0; i < a->getSize(); i++)
    {
        long long id = ids[0][i];
        long long card = ids[1][i];
        long long obf = math.mod(RandNum(id), modulo);
        if (card < 0)
            m = math.modAdd(m, math.modMul(obf, -card, modulo), modulo);
        else
            m = math.modSubtract(m, math.modMul(obf, card, modulo), modulo);
    }

    delete a;
    return handleNegative(m);
}

// @Override
//     long long SymAHE::decrypt(SymCipher& c) {
//         long long m = c.getValue();
//         vector <vector <long long >> ids = c.getIds();
//         for (int i = 0; i < c.getSize(); i++) {
//             long long id = ids[0][i];
//             long long card = ids[1][i];
//             long long obf=math.mod(RandNum(id), modulo);
//             if (card < 0)
//                 m = math.modAdd(m, math.modMul(obf, -card, modulo), modulo);
//             else
//                 m = math.modSubtract(m, math.modMul(obf, card, modulo), modulo);
//         }
//         return handleNegative(m);
//     }

SymCipher &SymAHE::add(SymCipher &c1, SymCipher &c2)
{

    c1.add(c2, modulo);
    return c1;
}

SymCipher &SymAHE::addPlaintext(SymCipher &c, long long m)
{
    c.addValue(m, modulo);
    return c;
}

SymCipher &SymAHE::multiply(SymCipher &c, long long m)
{
    c.multiply(m, modulo);
    return c;
}

SymCipher &SymAHE::neggate(SymCipher &c)
{
    return multiply(c, -1L);
}

SymCipher &SymAHE::subtract(SymCipher &c1, SymCipher &c2)
{
    return add(c1, neggate(c2));
}

// public String toString() {
//     return "<" + this.getClass().getSimpleName() + ">";
// }
