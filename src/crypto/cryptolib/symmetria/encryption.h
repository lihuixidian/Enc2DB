
#ifndef SYMMETRIA_2_ENCRYPTION_H
#define SYMMETRIA_2_ENCRYPTION_H
//
// Created by 甜甜 on 2020/12/2.
//

#include <string.h>
#include <memory>

#include "t.h"

using namespace std;

#define AES_BLOCK_SIZE 16
#define BUF_SIZE 1024
static const unsigned char en_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static unsigned char de_table[128] = {0};
class encryption
{
public:
    static void init_de_table();
    int get_encode_len(int len);
    int get_decode_len(int len);
    int base64_encode(const unsigned char *data, int data_len, char *dst, int dst_len);

    int base64_decode(const unsigned char *data, int data_len, char *dst, int dst_len);

    // 根据数据字节数 获取加密字节数
    int get_crypt_size(int data_len);

    // 加密
    //int aes_encrypt(char* in, char* key, char* out);
    int aes_encrypt(char *in, char *key, char *out, std::size_t inlen);
    // 解密
    int aes_decrypt(char *in, int in_len, char *key, char *out);

    long long charToLongLong(char b[]);
    //    char* DET_encrypt(char* m);
    char *DET_encrypt(char *m, std::size_t *cipher_length);
    char *DET_decrypt(char *ciphertext, std::size_t *out_len);
};

#endif //SYMMETRIA_2_ENCRYPTION_H
