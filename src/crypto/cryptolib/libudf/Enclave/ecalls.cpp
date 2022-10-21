

#include "KeyManager.h"
#include "LRUCache.hpp"
#include "SymAHE.h"
#include "SymMHE.h"
#include "Singleton.h"
#include "elogDefination.h"
#include "t.h"

#include <stdlib.h>
#include <tlibc/mbusafecrt.h>
#include <memory>

#include "Enclave_t.h"

#include <math.h>

std::unique_ptr<SymMHE> psmheCipher(new SymMHE());
std::unique_ptr<SymAHE> psaheCipher(new SymAHE());
std::unique_ptr<encryption> aesEncryption(new encryption());
std::unique_ptr<KeyManager> keyManager(new KeyManager()); //? Singleton 不能用？


constexpr int64_t CacheNValue = INT64_MIN;
#ifdef USE_LRU
std::unique_ptr<LRUCache<int64_t, int64_t>> aesCache(new LRUCache<int64_t, int64_t>());
#endif
int ereport(int errlevel, const char *fmt, ...);

// with cache check
int64_t decryptAESLong(char *hexstr)
{
#ifdef USE_LRU
    int64_t hashv = hash<char *>()(hexstr);
    int64_t ans = aesCache->get(hashv, CacheNValue);
#else
    int64_t ans = CacheNValue;
#endif
    if(ans == CacheNValue) {
    
        size_t asz = strlen(hexstr);

        t trans;
        size_t bufASz = asz / 2;

        char bufA[bufASz] = {0};
        char bufOut[aesEncryption->get_crypt_size(bufASz)];
        trans.hex2buf((uint8_t *)bufA, &bufASz, (const char *)hexstr, asz);
        string key = getAESKey();
        aesEncryption->aes_decrypt((char *)bufA, bufASz, (char *)key.c_str(), bufOut);
        ans = *(int64_t*)bufOut;
#ifdef USE_LRU
        aesCache->put(hashv, ans);
#endif
        }
    return ans;
}

#ifdef USE_LRU
// std::unique_ptr<LRUCache> lruCache(new LRUCache(64));

void ecall_getLruHit(double *d)
{
    // auto lruCache = Singleton<LRUCache<int64_t, int64_t>>::instance();
    // aesCache->setName("Enclave AES");
    *d = aesCache->hitRatio();
}

void ecall_getCacheSwapOut(double *d)
{
    // auto lruCache = Singleton<LRUCache<int64_t, int64_t>>::instance();
    // aesCache->setName("Enclave AES");
    *d = aesCache->swapOutRatio();
}


void ecall_fillLru()
{
    // SymAHE symPhe;
    // string key = getAESKey();
    // auto lruCache = Singleton<LRUCache<int64_t, int64_t>>::instance();

    // for (int i = 1; i <= lruCache->capacity() / 2; ++i)
    // {
    //     int64_t v = symPhe.RandNum(i);
    // }
    // lruCache->requestTimes = 0;
    // lruCache->hitTimes = 0;
}
#else
void ecall_fillLru()
{
}
void ecall_getLruHit(double *d) {}
void ecall_getCacheSwapOut(double *d) {}
#endif
void ecall_genMasterKey(const char *path)
{
    keyManager->setPath(path, "");
    keyManager->genMasterKey();
}

void ecall_openMasterKey(const char *path, void *out, size_t outlen)
{
    keyManager->setPath(path, "");
    keyManager->loadMasterKey();
    KeyManager::masterKey_t m;
    keyManager->masterKey(m);
    memcpy_s((void *)out, outlen, m, outlen);
}

void ecall_encryptKeyWithEnclave(const char *key, size_t keySize, const char *path)
{
    keyManager->setPath(path, "");
    keyManager->setMasterKey(key);
    keyManager->saveAllKeysToFile();
}

void ecall_sahe_add(void *a, void *b, void *c, size_t outlen)
{
    // ! a and b must be buf rather than hex stream

    // TODO: 改成aes
    // int64_t num1 = decryptAESLong(static_cast<char *>(a));
    // int64_t num2 = decryptAESLong(static_cast<char *>(b));

    int64_t num1 = decryptAESLong(static_cast<char *>(a));
    int64_t num2 = decryptAESLong(static_cast<char *>(b));
    int64_t num3 = num1 + num2;

    auto tmp = psaheCipher->encrypt(num3);

    assert(tmp && strlen(tmp) > 5 && tmp[0]);
    strcpy_s(static_cast<char *>(c), strlen(tmp) + 1, tmp);
    delete[] tmp;
}

void ecall_sahe_mul(void *a, int64_t b, void *c, size_t outlen)
{
    // ! a and b must be buf rather than hex stream

    SymMHE *psaheCipher = new SymMHE();
    int64_t num1 = decryptAESLong(static_cast<char *>(a));
    // int64_t num2 = decryptAESLong(static_cast<char *>(b));

    int64_t num3 = num1 * b;

    auto tmp = psaheCipher->encrypt(num3);
    strcpy_s(static_cast<char *>(c), strlen(tmp), tmp);

    delete[] tmp;
}

void ecall_sahe_sub(void *a, void *b, void *c, size_t outlen)
{
    // ! a and b must be buf rather than hex stream

    int64_t num1 = decryptAESLong(static_cast<char *>(a));
    int64_t num2 = decryptAESLong(static_cast<char *>(b));

    int64_t num3 = num1 - num2;

    auto tmp = psaheCipher->encrypt(num3);
    strcpy_s(static_cast<char *>(c), strlen(tmp) + 1, tmp);

    delete[] tmp;
}

void ecall_sahe_neg(void *a, void *c, size_t outlen)
{
    // ! a and b must be buf rather than hex stream

    int64_t num1 = decryptAESLong(static_cast<char *>(a));
    int64_t num3 = -num1;
    auto tmp = psaheCipher->encrypt(num3);
    strcpy_s(static_cast<char *>(c), strlen(tmp) + 1, tmp);
    delete[] tmp;
}

void ecall_smhe_mul(void *a, void *b, void *c, size_t outlen)
{
    // ! a and b must be buf rather than hex stream

    int64_t num1 = decryptAESLong(static_cast<char *>(a));
    int64_t num2 = decryptAESLong(static_cast<char *>(b));

    int64_t num3 = num1 * num2;

    auto tmp = psmheCipher->encrypt(num3);
    strcpy_s(static_cast<char *>(c), strlen(tmp) + 1, tmp);

    delete[] tmp;
}

void ecall_smhe_mulp(void *a, int64_t b, void *c, size_t outlen)
{
    // ! a and b must be buf rather than hex stream

    int64_t num1 = decryptAESLong(static_cast<char *>(a));
    int64_t num3 = num1 * b;
    auto tmp = psmheCipher->encrypt(num3);
    strcpy_s(static_cast<char *>(c), strlen(tmp) + 1, tmp);

    delete[] tmp;
}

void ecall_smhe_div(void *a, void *b, void *c, size_t outlen)
{
    // ! a and b must be buf rather than hex stream

    int64_t num1 = decryptAESLong(static_cast<char *>(a));
    int64_t num2 = decryptAESLong(static_cast<char *>(b));

    int64_t num3 = num1 / num2;

    auto tmp = psmheCipher->encrypt(num3);
    strcpy_s(static_cast<char *>(c), strlen(tmp) + 1, tmp);

    delete[] tmp;
}

void ecall_smhe_pow(void *a, int64_t b, void *c, size_t outlen)
{
    // ! a and b must be buf rather than hex stream

    int64_t num1 = decryptAESLong(static_cast<char *>(a));
    int64_t num3 = pow(num1, b);

    auto tmp = psmheCipher->encrypt(num3);
    strcpy_s(static_cast<char *>(c), strlen(tmp) + 1, tmp);
    delete[] tmp;
}

void ecall_smhe_inv(void *a, void *c, size_t outlen)
{
    // ! a and b must be buf rather than hex stream

    int64_t num1 = decryptAESLong(static_cast<char *>(a));
    int64_t num3 = 1 / num1;
    auto tmp = psmheCipher->encrypt(num3);
    strcpy_s(static_cast<char *>(c), strlen(tmp) + 1, tmp);
    delete[] tmp;
}

void ecall_ore_compare(void *a, void *b, int *c)
{

    int64_t num1 = decryptAESLong(static_cast<char *>(a));
    int64_t num2 = decryptAESLong(static_cast<char *>(b));
    if (num1 == num2)
        *c = 0;
    else if (num1 > num2)
        *c = 1;
    else
        *c = -1;
}

int ereport(int errlevel, const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_ereport(buf);
    return (int)strnlen(buf, BUFSIZ - 1) + 1;
}