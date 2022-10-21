#ifndef CRYPTOSCHEME_H
#define CRYPTOSCHEME_H
#include <memory>

#include <map>
#include <vector>
#include <string.h>
#include "Symcipher.h"

using namespace std;

class CryptoScheme
{

public:
    static const int DEFAULT_NEGDIVISOR = 2;
    //**********************************************
    //static final SecureRandom RNG = new SecureRandom();
    //**********************************************

    //**********************************************
    //final string publicKeyPath;
    //**********************************************
    //const string privateKeyPath;
    //**********************************************

    //    string publicKeyPath;
    //    string privateKeyPath;

    //final Object publicKey;
    //final Object privateKey;
    //**********************************************
    static const string publicKey;
    static const string privateKey;

    bool isSymmetric{};

public:
    CryptoScheme();
    //CryptoScheme(string privateKeyPath);
    //    public:
    //    CryptoScheme(string publicKeyPath, string privateKeyPath);

    /**
     * Check whether the required keys for the encryption scheme exist.
     *
     * @return true if the required keys exist and false otherwise
     */
    //    private:
    //    bool keysExist();

    /**
     * Generate a key for the cipher.
     */

    //public:
    //virtual void keyGen();
};

#endif