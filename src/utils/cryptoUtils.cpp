

#ifdef __cplusplus
extern "C"
{
#endif

#include "postgres.h" // 与 #include <iomanip> 冲突

#include "utils/palloc.h"
#include "utils/memutils.h"
#include <string.h>
#ifdef __cplusplus
}
#endif

#include <fstream>
#include <algorithm>
#include <cctype>
#include <memory>
#include "crypto/crypto.h"

using namespace std;

const char *typeCipher2String(T_Cipher t)
{
    switch (t)
    {
    case CIPHER_SAHE:
        return "_SAHE";

    case CIPHER_SMHE:
        return "_SMHE";

    case CIPHER_DET:
        return "_DET";

    case CIPHER_ORE:
        return "_ORE";

    case CIPHER_AES:
        return "_AES";

    case CIPHER_NOCRYPT:
        return "_NOCRYPT";

    default:
        return "_Unknown";
    }
}

T_Cipher string2TypeCipher(const char *str)
{

    shared_ptr<char> sharedStr(new char[strlen(str) + 1](), [](char *p)
                               { delete[] p; });

    transform(str, str + strlen(str) + 1, sharedStr.get(), ::toupper);
    str = (const char *)sharedStr.get();
    if (!strcmp(str, "SAHE"))
    {
        return CIPHER_SAHE;
    }

    else if (!strcmp(str, "SMHE"))
    {
        return CIPHER_SMHE;
    }
    // else if (!strcmp(str, "DET"))
    // {
    //     return CIPHER_DET;
    // }
    else if (!strcmp(str, "ORE"))
    {
        return CIPHER_ORE;
    }
    else if (!strcmp(str, "AES"))
    {
        return CIPHER_AES;
    }
    // else if (!strcmp(str, "NOCRYPT"))
    // {
    //     return CIPHER_NOCRYPT;
    // }

    return CIPHER_NOCRYPT;
}

size_t getCipherBufSize(T_Cipher t)
{
    switch (t)
    {
    case CIPHER_SAHE:
        return CIPHER_SAHE_SIZE;

    case CIPHER_SMHE:
        return CIPHER_SMHE_SIZE;

    case CIPHER_DET:
        return CIPHER_DET_SIZE;

    case CIPHER_ORE:
        return CIPHER_ORE_SIZE;

    case CIPHER_AES:
        return CIPHER_AES_SIZE;
    }
    return 0;
}