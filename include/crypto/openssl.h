#pragma once



int genRandomBytes(uint8_t* randombytes, size_t size);
int buf2hex(const uint8_t* buf, size_t bufSize, char *str, size_t* strlen);
int hex2buf(uint8_t* buf, size_t* bufSize, const char* strhex, size_t hexstrlen);
// Error
#define ERR_OPEN_FAILED     -1
#define ERR_BAD_MODE        -2
#define ERR_NEW_FAILED      -3
#define ERR_RUN_FAILED      -4
#define ERR_READ_FAILED     -5
#define ERR_STATUS_UNINIT   -6