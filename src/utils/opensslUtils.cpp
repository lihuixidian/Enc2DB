

#include <sstream>

#include <iomanip>
#include <string>
#include <string.h>

using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/buffer.h>

<<<<<<< HEAD:src/crypto/openssl.cpp
#include "crypto/base64.h"
=======
#include "base64.h"
>>>>>>> dbdev:src/utils/opensslUtils.cpp

#ifdef __cplusplus
}
#endif
#include "crypto/openssl.h"

void openSSLErrHandle()
{
    auto e = ERR_peek_last_error();
    char *c_err_str = (char *)ERR_reason_error_string(e);
    if (c_err_str)

        ERR_print_errors_fp(stderr);
    ERR_clear_error();
}

int genRandomBytes(uint8_t *randombytes, size_t size)
{

    if (RAND_bytes(randombytes, size) <= 0)
    {
        openSSLErrHandle();
        return -1;
    }
    else
        return 0;
}

int buf2hex(const uint8_t *buf, size_t bufSize, char *hexstr, size_t *hexstrlen)
{ // need to alloc hexstr by yourself

    if (*hexstrlen < bufSize * 2)
        return -1;

    stringstream ss;

    for (int i = 0; i < bufSize; ++i)
        ss << setfill('0') << setw(2) << hex << (0xff & (unsigned char)buf[i]);
    string mystr = ss.str() + '\0';
    strcpy(hexstr, mystr.c_str());
    *hexstrlen = mystr.size();
    return 0;
}

int hex2buf(uint8_t *buf, size_t *bufSize, const char *strhex, size_t hexstrlen) //  // need to alloc buf by yourself
{
    if (*bufSize < hexstrlen / 2)
        return -1;

    for (unsigned int i = 0; i < hexstrlen; i += 2)
    {
        std::string byteString(strhex + i, strhex + i + 2);
        uint8_t byte = (uint8_t)strtol(byteString.c_str(), NULL, 16);
        buf[i / 2] = byte;
    }

    *bufSize = hexstrlen / 2;

    return 0;
}

// void testBufConversion() __attribute__((visibility("default")));
// void testBufConversion()
// {
//     char c[] = "auzxichqheqownbiuebuaihsg";
//     size_t bufsz = sizeof(c) + 1;
//     size_t hexsz = 2 * (sizeof(c) + 1);
//     char* h = new char[hexsz * 2];

//     char* nc = new char[hexsz];

//     buf2hex((uint8_t*)c, strlen(c), h, &hexsz);
//     hex2buf((uint8_t*)nc, &hexsz, h, hexsz);

//     // buf:" << c << endl << "hex string:" << h << endl << "convted c: "  << nc << endl;
// }
