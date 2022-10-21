#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crypto/openssl.h"
// #include "base64.h"

<<<<<<< HEAD
#define DATABASE_CIPHER_TYPE "text"

#define CIPHER_FORMAT "hexstr"  // or "base64"

// #define USER_PRIMARY_KEY    "/etc/encryptsql/userKey"

static constexpr int ORE_nbits = 32;
static constexpr int ORE_block_len = 8;
=======
#include "settings.h"
>>>>>>> dbdev

// #define CIPHER_COUNT 5
typedef enum
{
    CIPHER_SAHE,
    CIPHER_SMHE,

    CIPHER_ORE,
    CIPHER_AES,
    CIPHER_NOCRYPT, // for such 'and', 'or' oprator or column that do not encrypt use
    CIPHER_MAPPED,  // for such 'table', 'column name' use

    CIPHER_COUNT,
    CIPHER_DET // use CIPHER_AES instead since they are the same

} T_Cipher;

const char *typeCipher2String(T_Cipher t);
size_t getCipherBufSize(T_Cipher t);
T_Cipher string2TypeCipher(const char *str);
uint8_t *encryptValue(T_Cipher encryptCipher, uint8_t *in_text, size_t in_size, size_t *out_size, bool isEncrypt = true, bool isFloat = false);
size_t getCipherFormatSize(const char *fmt, T_Cipher t);

// string getAESKey();
