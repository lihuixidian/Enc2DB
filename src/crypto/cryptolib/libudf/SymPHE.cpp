
#include <stdlib.h>
#include <memory>
#include <stdio.h>

#include <map>
#include <utility>
#include <vector>
#include <string.h>
#include "Symcipher.h"
#include "MathUtils.h"
#include "ArraySymCipher.h"
#include "CryptoScheme.h"
#include "SymPHE.h"
#include "encryption.h"

#include "KeyManager.h"
#include "Singleton.h"
#include "LRUCache.hpp"

#include <fstream>
#include <sstream>

using namespace std;

long long SymPHE::nextId = 0;

// used to generate random numbers
//*******************************
// Cipher aesBlockCipher;
//*******************************

SymPHE::SymPHE() = default;

SymPHE::SymPHE(CipherType cipherType, int negDivisor)
{
    //*******************************
    //CryptoScheme(privateKeyPath);
    //*******************************

    this->cipherType = cipherType;

    ////*******************************
    //setupRandNum();
    ////*******************************

    // nextId = 1;
}

/**
 * Returns a cipher of the given type.
 */
SymCipher *SymPHE::generateCipher(CipherType cipherType, long long value, long long id)
{
    //SymCipher cipher1=ArraySymCipher(value, id);
    SymCipher *cipher = new ArraySymCipher(value, id);

    //*******************************
    //cipher = new ArraySymCipher(value, id);
    //*******************************

    //*******************************
    // else if (cipherType == CipherType.RANGE)
    //     cipher = new RangeSymCipher(value, id);
    //*******************************
    return cipher;
}

/**
 * Encrypt the given message
 */
char *encrypt(long long message);

/**
 * Decrypt the given ciphertext.
 */
long long decrypt(char *ciphertext);

/**
 * Returns the next id to use to encrypt.
 */



long long SymPHE::getNextId()
{
// #ifdef USE_LRU
//     return (SymPHE::nextId++ );
// #else 
//     return ++SymPHE::nextId;
// #endif
    return ++SymPHE::nextId;
}

/**
 * Returns a positive long long number in the range 0-n. The number is generated using a keyed random
 * number generator.
 */
// public:
// long long getRandNum(long long id, long long modulo) {
//     byte[] b = new byte[0];
//     try {
//         b = aesBlockCipher.doFinal(String.valueOf(id).getBytes());
//     } catch (Exception e) {
//         e.printStackTrace();
//     }
//     return MathUtils.mod(ByteUtils.bytesTolong long(b), modulo);// ByteUtils.bytesTolong long(b) Converts a byte array to a long long number. Uses the first 8 bytes.
// }

/**
 * Setup cipher used for generating random numbers
 */
// public:
// void setupRandNum() {
//     String algorithm = "AES";
//     String key = (String) privateKey;
//     try {
//         byte[] keyBA = key.getBytes(StandardCharsets.UTF_8);
//         MessageDigest sha = MessageDigest.getInstance("SHA-1");
//         keyBA = sha.digest(keyBA);
//         keyBA = Arrays.copyOf(keyBA, 16);
//         //获取密钥

//         SecretKeySpec secretKeySpec = new SecretKeySpec(keyBA, algorithm);
//         aesBlockCipher = Cipher.getInstance(algorithm);
//         aesBlockCipher.init(Cipher.ENCRYPT_MODE, secretKeySpec);

//         //加上getRandNum里面的do final 最终获取密文
//     } catch (InvalidKeyException | NoSuchAlgorithmException | NoSuchPaddingException e) {
//         e.printStackTrace();
//     }
// }

/**
 * Setup the threshold between positive and negative numbers.
 */

/**
 * Shift the given message to allow representing negative numbers.
 */

extern string getAESKey();

#ifdef USE_LRU
// LRUCache<long, long> HECache; 
#endif

long long SymPHE::RandNum(long long id)
{
    string key = getAESKey();
    auto aes_keybuf = (char*)key.c_str();
#ifdef USE_LRU

    // static auto lruCache = LRUCache<long, long>>::instance();
    // lruCache->setName("Client HE");
    // long long ret = HECache.get(id, -1);
    // if (ret != -1)
    //     return ret;
#endif

    char buf[32];
    memset(buf, 1, sizeof(buf));
    string id1 = to_string(id);
    const char *c_id = id1.c_str();
#ifdef ENCLAVE
    strcpy_s(buf, sizeof(buf), c_id);
#else
    strcpy(buf, c_id);
#endif
    std::size_t inlen;
    inlen = sizeof(buf);
    //std::// plain value is :" << buf << std::endl;


    //std::// key value is :" << aes_keybuf << std::endl;

    char aes_en[BUF_SIZE] = {0};

    int aes_en_len = en.aes_encrypt(buf, aes_keybuf, aes_en, inlen); // aes-128 加密
    if (aes_en_len < 0)
    {
        //        printf("aes_encrypt error ret: %d\n", aes_en_len);
        //        return -1;
        throw "aes_encrypt error !";
    }

    long long a = en.charToLongLong(aes_en);
#ifdef USE_LRU
    // HECache.put(id, a);
#endif
    return a;
}
