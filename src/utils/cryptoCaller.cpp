

extern "C"
{

#include "postgres.h" // 与 #include <iomanip> 冲突

#include "utils/palloc.h"
#include "utils/memutils.h"

#include "ore_crypto.h"
#include "ore_blk.h"
#include "errors.h"
#include "ORE_STR22STRUCT.h"
}

#include "crypto/crypto.h"

#include <string>
#include <memory>
#include <string>
#include <unordered_set>

#include "MathUtils.h"
#include "Symcipher.h"
#include "ArraySymCipher.h"
#include "str22struct.h"
#include "SymPHE.h"
#include "SymAHE.h"
#include "SymMHE.h"
#include "t.h"
#include "KeyManager.h"
#include "TimeCounter.h"
#include "Singleton.h"
#include <iostream>

using std::shared_ptr;
using std::string;
using std::unordered_set;


extern string ctext_to_str(char **dest, ore_blk_ciphertext cipher);
uint8_t *encryptValue(
    T_Cipher encryptCipher, uint8_t *in_text, size_t in_size, size_t *out_size, bool isEncrypt, bool isFloat) // this function will alloc memory
{
    *out_size = 0;

    TimeCounter *counter = Singleton<TimeCounter>::instance();

    shared_ptr<uint8_t> outtext(nullptr, [](uint8_t *p)
                                { delete[] p; });
    shared_ptr<uint8_t> aesOut(new uint8_t[BUF_SIZE], [](uint8_t *p)
                               { delete[] p; });

    char *aesKey = nullptr;

    string aesK = getAESKey();
    aesKey = (char *)aesK.c_str();

    // #if defined(SGX_ORE) || defined(SGX_MHE)
    // #else
    // #endif
    if (isEncrypt && (encryptCipher == CIPHER_SAHE || encryptCipher == CIPHER_SMHE || encryptCipher == CIPHER_ORE))
    {
        if (in_size != sizeof(long))
        { // maybe a float
            double tmp = atof((const char *)in_text);
            long longTmp = static_cast<long>(tmp);
            in_text = (uint8_t *)&longTmp;
            in_size = sizeof(long);
        }
    }

    if ((encryptCipher == CIPHER_AES || encryptCipher == CIPHER_DET) && (in_size % AES_BLOCK_SIZE != 0 || in_size < AES_BLOCK_SIZE))
    {
        int cur_size = 0; // AES扩展明文成BLOCK倍

        int tmp_size = (in_size < AES_BLOCK_SIZE) ? AES_BLOCK_SIZE : in_size;
        while (tmp_size > 0)
        {
            cur_size += AES_BLOCK_SIZE;
            tmp_size -= AES_BLOCK_SIZE;
        }
        char *tmp_text = (char *)palloc0(cur_size);
        memcpy(tmp_text, (char *)in_text, in_size);
        in_text = (uint8_t *)tmp_text;
        in_size = cur_size;
    }

    if (isEncrypt)
    {
        switch (encryptCipher)
        {
        case CIPHER_SAHE:
        {
            SymAHE symAHE = SymAHE();
            if (in_size != sizeof(long))
                throw "in size does not match with long size in sahe";
            Timer t;
            outtext.reset((uint8_t *)symAHE.encrypt(*(long *)in_text));
            counter->count("SAHE Encryption", t.passedTimeMicroSecond());

            *out_size = strlen((const char *)outtext.get());
            break;
        }
        case CIPHER_SMHE:
        {
#ifdef SGX_MHE
            encryption encryptCollection;
            if (!aesKey)
                throw "cannot get AES key\n";
            Timer t;

            *out_size = encryptCollection.aes_encrypt((char *)in_text, (char *)aesKey, (char *)aesOut.get(), in_size);

            counter->count("ORE_AES Encryption", t.passedTimeMicroSecond());

            outtext = aesOut;
            break;
#else
            SymMHE symMHE = SymMHE();
            if (in_size != sizeof(long))
                throw "in size does not match with long size in smhe";

            Timer t;
            outtext.reset((uint8_t *)symMHE.encrypt(*(long *)in_text));

            counter->count("SMHE Encryption", t.passedTimeMicroSecond());
            *out_size = strlen((const char *)outtext.get());
            break;
#endif
        }

        case CIPHER_DET:

        case CIPHER_AES:
        {
            encryption encryptCollection;
            if (!aesKey)
                throw "cannot get AES key\n";
            Timer t;

            *out_size = encryptCollection.aes_encrypt((char *)in_text, (char *)aesKey, (char *)aesOut.get(), in_size);

            counter->count("AES Encryption", t.passedTimeMicroSecond());

            outtext = aesOut;
            break;
        }

        case CIPHER_ORE:
        {

#if defined(SGX_ORE) || defined(SGX_MHE)
            encryption encryptCollection;
            if (!aesKey)
                throw "cannot get AES key\n";
            Timer t;

            *out_size = encryptCollection.aes_encrypt((char *)in_text, (char *)aesKey, (char *)aesOut.get(), in_size);

            counter->count("ORE_AES Encryption", t.passedTimeMicroSecond());

            outtext = aesOut;
            break;
#else
            Timer t;
            ore_blk_params params;
            init_ore_blk_params(params, ORE_nbits, ORE_block_len);

            ore_blk_secret_key sk;
            ore_blk_setup(sk, params);

            ore_blk_ciphertext ctxt1;
            init_ore_blk_ciphertext(ctxt1, params);

            ore_blk_encrypt_ui(ctxt1, sk, *(long *)in_text);

            char *__unused = nullptr;
            // ore_blk_ciphertext cipher2;
            string str = ctext_to_str(&__unused, ctxt1);

            outtext.reset(new uint8_t[str.size() + 1]());

            counter->count("ORE Encryption", t.passedTimeMicroSecond());

            memcpy(outtext.get(), str.c_str(), str.size());

            *out_size = str.size();
            break;
#endif
        }
        case CIPHER_NOCRYPT:

            outtext.reset(new uint8_t[in_size]());
            memcpy(outtext.get(), in_text, in_size);
            *out_size = in_size;
            break;

            // case CIPHER_NOCRYPT:
            //     outtext = in_text;
            //     *out_size = strlen((const char *)outtext);
            //     break;
        }
    }
    else
    { // decrypt

        switch (encryptCipher)
        {
        case CIPHER_SAHE:
        {
            in_text[in_size - 1] = 0; // 将浮点数标识位取消
            SymAHE symAHE = SymAHE();
            long *tmp = new long;
            Timer t;
            *tmp = symAHE.decrypt((char *)in_text);
            counter->count("SAHE Decryption", t.passedTimeMicroSecond());

            outtext.reset((uint8_t *)tmp);
            *out_size = sizeof(long);
            break;
        }

        case CIPHER_SMHE:
        {
            in_text[in_size - 1] = 0; // 将浮点数标识位取消
            SymMHE symMHE = SymMHE();
            long long *tmp = new long long;
            Timer t;
            *tmp = symMHE.decrypt((char *)in_text);
            counter->count("SMHE Decryption", t.passedTimeMicroSecond());

            outtext.reset((uint8_t *)tmp);
            *out_size = sizeof(long);

            break;
        }

        case CIPHER_DET:

        case CIPHER_AES:
        {
            encryption encryptCollection;
            Timer t;
            *out_size = encryptCollection.aes_decrypt((char *)in_text, in_size, (char *)aesKey, (char *)aesOut.get());
            counter->count("AES Decryption", t.passedTimeMicroSecond());

            outtext = aesOut;
            break;
        }
        case CIPHER_NOCRYPT:

            // outtext = encryptCollection. in_text;
            outtext.reset(new uint8_t[in_size]());
            memcpy(outtext.get(), in_text, in_size);
            *out_size = in_size;
            // outtext = in_text;
            break;
        case CIPHER_ORE:
            // ! ORE has No Decrypt!
            ereport(FATAL,
                    (errcode(ERRCODE_CANNOT_CONNECT_NOW),
                     errmsg("Can not decrypt ORE!\n")));
            exit(-1);
            break;
        }
    }

    char *pallocbuf = (char *)palloc0(*out_size + 2);
    memset(pallocbuf, 0, *out_size + 2);
    memcpy(pallocbuf, (const char *)outtext.get(), *out_size);
    size_t cipherSize = strlen((char *)outtext.get());
    if (encryptCipher == CIPHER_SAHE || encryptCipher == CIPHER_SMHE)
    {
        //将密文的最后一位用作标识是否是浮点数
        if (isEncrypt)
        {
            pallocbuf[cipherSize] = isFloat ? '2' : '1';
        }
        (*out_size) += 2;
    }

    return (uint8_t *)pallocbuf;
}

size_t getCipherFormatSize(const char *fmt, T_Cipher t)
{
    if (!strcmp("hexstr", fmt))
        return getCipherBufSize(t) * 2 + 1;
    if (!strcmp("base64", fmt))
        return getCipherBufSize(t) * 1.4 + 1;
    return -1;
}
