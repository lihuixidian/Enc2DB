#ifndef SYMPHE_H
#define SYMPHE_H
#include <memory>
//
#include <map>
#include <vector>
#include <string.h>
#include "Symcipher.h"
#include "MathUtils.h"
#include "ArraySymCipher.h"
#include "CryptoScheme.h"
#include "encryption.h"

#include <fstream>
#include <sstream>

//*******************************
//#include "RangeSymcipher.h"
//*******************************

//*******************************
//#include "ByteUtils.h"
//*******************************

using namespace std;

class SymPHE : public CryptoScheme
{

    //*******************************
    // static const CipherType DEFAULT_CIPHER_TYPE = CipherType.ARRAY;
    //*******************************

    // static const CipherType DEFAULT_CIPHER_TYPE = CipherType.ARRAY;
    // arithmetic modulo

    // SymCipher sym;

public:
    // static const long long modulo = 9223372036854775783L;
    // static const long long modulo = LLONG_MAX;
    long long modulo{};

    encryption en;
    // public BigInteger moduloBI; // modulo as a BI
    //*******************************
    // public BigInteger moduloBI;
    //*******************************

    // threshold that separates positive from negative numbers
    long long negThreshold;

    // what type of cipher to use. This includes the method to use to handle id lists.
    CipherType cipherType;

    // the next id to use
    static long long nextId;

    // used to generate random numbers

    //*******************************
    // Cipher aesBlockCipher;
    //*******************************

public:
    SymPHE();
    SymPHE(CipherType cipherType, int negDivisor);

    /**
     * Returns a cipher of the given type.
     */
    static SymCipher *generateCipher(CipherType cipherType, long long value, long long id);

    /**
     * Encrypt the given message
     */
    virtual char *encrypt(long long message) = 0;

    /**
     * Decrypt the given ciphertext.
     */
    virtual long long decrypt(char *c) = 0;

    /**
     * Returns the next id to use to encrypt.
     */
public:
    long long getNextId();

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
    //     return MathUtils.mod(ByteUtils.bytesTolong long(b), modulo);// ByteUtils.bytesTolong long(b) Converts a byte
    //     array to a long long number. Uses the first 8 bytes.
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
public:
    virtual void setupNegative(int negDivisor) = 0;

    /**
     * Shift the given message to allow representing negative numbers.
     */
    virtual long long handleNegative(long long m) = 0;

    long long RandNum(long long id);
};

#endif