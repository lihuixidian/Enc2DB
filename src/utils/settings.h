#pragma once

// 明文的数据类型， 依据不同的数据类型选择加密方案。
#define MAX_PLAIN_STRING_TYPE 4

#define NOEXIST "NNnoexistNN" // table or column not found in mapper

constexpr char CONFIG_PATH[] = "/etc/encryptsql/config.json";
constexpr double Float_Scale = 10000.0;
constexpr char ENCLAVE_FILENAME[] = "/etc/encryptsql/enclave.signed.so";

// crypto.h things
#define DEFALUT_ENCRYPT_CIPHER CIPHER_AES     // select target默认的加密列
#define DEFALUT_STAR_EXPR_CIPHER CIPHER_AES   // 星号表达式 默认选择的列
#define DEFALUT_PRIMARY_KEY_CIPHER CIPHER_AES // 主键约束 默认的加密列

#define CIPHER_SAHE_SIZE 2048 / 8;
#define CIPHER_SMHE_SIZE 2048 / 8;
#define CIPHER_DET_SIZE 2048 / 8;
#define CIPHER_ORE_SIZE 2048 / 8;
#define CIPHER_AES_SIZE 2048 / 8;

#define DATABASE_CIPHER_TYPE "text"

#define CIPHER_FORMAT "hexstr" // or "base64"

// #define USER_PRIMARY_KEY    "/etc/encryptsql/userKey"

constexpr int ORE_nbits = 32;
constexpr int ORE_block_len = 8;