//
// Created by 甜甜 on 2020/12/2.
//

#include "encryption.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string.h>
#include <memory.h>
#include <openssl/aes.h>
#include <stdlib.h>
#include "t.h"

using namespace std;



void encryption::init_de_table()
{
    if (de_table[0] != 0) {
        return;
    }
    memset(de_table, 0xff, sizeof(de_table));
    const unsigned char* p;
    for (p = en_table; *p != 0; p++) {
        de_table[*p] = p - en_table;
    }
    de_table['='] = 0;
}

int encryption::get_encode_len(int len)
{
    return (((len/3 + (len%3==0 ? 0:1)) * 4) + 1);
}

int encryption::get_decode_len(int len)
{
    return ((len/4)*3 + 1);
}

// base64 编码
int encryption::base64_encode(const unsigned char* data, int data_len, char* dst, int dst_len)
{
    if (data == NULL || data_len == 0 || dst == NULL || dst_len == 0) {
        return -1;
    }

    if (dst_len < get_encode_len(data_len)) {
        return -2;
    }

    int mod_len = data_len%3;

    char* dst_tmp = dst;

    int pos = 0;
    for (pos = 0; pos+2 < data_len; pos+=3) {
        *(dst_tmp++) = en_table[data[pos] >> 2];
        *(dst_tmp++) = en_table[((data[pos] & 0x03) << 4) | (data[pos+1] >> 4)];
        *(dst_tmp++) = en_table[((data[pos+1] & 0x0f) << 2) | (data[pos+2] >> 6)];
        *(dst_tmp++) = en_table[data[pos+2] & 0x3f];
    }

    if (mod_len == 2) {
        *(dst_tmp++) = en_table[data[pos] >> 2];
        *(dst_tmp++) = en_table[((data[pos] & 0x03) << 4) | (data[pos+1] >> 4)];
        *(dst_tmp++) = en_table[((data[pos+1] & 0x0f) << 2)];
        *(dst_tmp++) = '=';
    }
    else if (mod_len == 1) {
        *(dst_tmp++) = en_table[data[pos] >> 2];
        *(dst_tmp++) = en_table[((data[pos] & 0x03) << 4)];
        *(dst_tmp++) = '=';
        *(dst_tmp++) = '=';
    }

    *dst_tmp = 0;

    return (dst_tmp - dst);
}

// base64 解码
int encryption::base64_decode(const unsigned char* data, int data_len, char* dst, int dst_len)
{
    if (data == NULL || data_len < 4 || dst == NULL || dst_len == 0) {
        return -1;
    }

    if (dst_len < get_decode_len(data_len)) {
        return -2;
    }

    init_de_table();

    char* dst_tmp = dst;
    int pos;
    for (pos = 0; pos < data_len; pos+=4) {
        if (de_table[data[pos]]==0xff || de_table[data[pos+1]]==0xff
            || de_table[data[pos+2]]==0xff || de_table[data[pos+3]]==0xff)
        {
            return -3;
        }
        *(dst_tmp++) = (de_table[data[pos]]<<2) | (de_table[data[pos+1]]>>4)&0x3;
        *(dst_tmp++) = (de_table[data[pos+1]]<<4) | (de_table[data[pos+2]]>>2)&0x0f;
        *(dst_tmp++) = (de_table[data[pos+2]]<<6) | (de_table[data[pos+3]])&0x3f;
    }

    while (*(dst_tmp-1) == 0) {
        dst_tmp--;
    }

    *dst_tmp = 0;

    return (dst_tmp - dst);
}

// 根据数据字节数 获取加密字节数
int encryption::get_crypt_size(int data_len)
{
    return ((data_len/AES_BLOCK_SIZE)+((data_len%AES_BLOCK_SIZE) == 0 ? 0:1))*AES_BLOCK_SIZE;
}

// 加密
int encryption::aes_encrypt(char* in, char* key, char* out,size_t inlen)//, int olen)
{
    if(NULL == in || NULL == key || NULL == out)
        return -1;

    AES_KEY aes;
    if(AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return -2;
    }

    //int len=strlen(in);
    int en_len=0;
    while(en_len<inlen)//输入输出字符串够长。而且是AES_BLOCK_SIZE的整数倍，须要严格限制
    {
        AES_encrypt((unsigned char*)in, (unsigned char*)out, &aes);
        in+=AES_BLOCK_SIZE;
        out+=AES_BLOCK_SIZE;
        en_len+=AES_BLOCK_SIZE;
    }

    return get_crypt_size(inlen);
}

// 解密
int encryption::aes_decrypt(char* in, int in_len, char* key, char* out)
{
    if(NULL == in || NULL == key || NULL == out)
        return -1;

    AES_KEY aes;
    if(AES_set_decrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return -2;
    }

    int en_len=0;
    while(en_len<in_len)
    {
        AES_decrypt((unsigned char*)in, (unsigned char*)out, &aes);
        in+=AES_BLOCK_SIZE;
        out+=AES_BLOCK_SIZE;
        en_len+=AES_BLOCK_SIZE;
    }

    return en_len;
}

long long encryption::charToLongLong(char b[])
{
    // long long result=0L;
    // for(int i=0;i<sizeof(long long);i++)
    // {
    //     result<<=
    // }
    return*((long long *)b);
}

char * encryption::DET_encrypt(char* m, size_t * cipher_length)
{
    //encryption encrypt;
    t change;
    char buf[100];
    memset(buf, 1, sizeof(buf));
    strcpy((char*)buf, m);
    size_t inlen;
    //std::cout << "current plain value is :" << buf << std::endl;
    std::cout << "current plain value is :" << buf << std::endl;
    std::cout << "length is :" << strlen(buf) << std::endl;
    inlen=strlen(buf);

    char aes_keybuf[32];
    memset(aes_keybuf, 0, sizeof(aes_keybuf));


    ifstream infile;
    infile.open("SAHE_key.txt");
    if (!infile.is_open())
        cout << "open file failure" << endl;
    while (!infile.eof())
    {
        infile.getline(aes_keybuf,32);

    }
    infile.close();

    std::cout << "current aes_keybuf value is :" << aes_keybuf << std::endl;
    char *aes_en=new char[BUF_SIZE]();
    //char aes_en[BUF_SIZE] = {0};

    int aes_en_len = aes_encrypt(buf, aes_keybuf, aes_en,inlen);
    *cipher_length = aes_en_len;

    // aes-128 加密
//    if (aes_en_len < 0)
//    {
//        printf("aes_encrypt error ret: %d\n", aes_en_len);
//        return -1;
//    }

    // size_t bufsz1 = strlen(aes_en) + 1;
    // size_t hexsz1 = 2 * (strlen(aes_en) + 1);
    // char* h = new char[hexsz1 * 2];
    // change.buf2hex((uint8_t*)aes_en, strlen(aes_en), h, &hexsz1);

    return aes_en;


}

char* encryption::DET_decrypt(char* ciphertext, std::size_t *cipher_len)

{
    encryption encrypt;
    t change;
    // size_t bufsz2 = sizeof(ciphertext) + 1;
    // size_t hexsz2 = 2 * (strlen(ciphertext) + 1);
    // char* nc2 = new char[hexsz2];
    // change.hex2buf((uint8_t*)nc2, &hexsz2, ciphertext, hexsz2);
    // cout<<"cipher1_parse: "<<nc2<<endl;
    char *aes_de=new char[BUF_SIZE]();
    //char aes_de[BUF_SIZE] = {0};
    char aes_keybuf[32];
    memset(aes_keybuf, 0, sizeof(aes_keybuf));
    //memset(aes_keybuf, 0, sizeof(aes_keybuf));

    ifstream infile2;
    infile2.open("SAHE_key.txt");
    if (!infile2.is_open())
        cout << "open file failure" << endl;
    while (!infile2.eof())
    {
        infile2.getline(aes_keybuf,32);

    }
    infile2.close();

    //strcpy((char*)aes_keybuf, CRYPT_USER_KEY);

    //const   char*   p2   =   (const   char*)(char*)aes_de;
    int aes_de_len = encrypt.aes_decrypt(ciphertext, strlen(ciphertext), aes_keybuf, aes_de); // aes-128 解密
    *cipher_len = aes_de_len;
    // if (aes_de_len < 0)
    // {
    //     printf("aes_encrypt error ret: %d\n", aes_de_len);
    //     return -1;
    // }
    printf("aes_de_len: %d aes_de: %s\n", strlen(aes_de), aes_de);
    cout<<"length: "<<strlen(aes_de)<<endl;

//    char * plaintext=new char [strlen(aes_de)+1];
//
//    cout<<"length: "<<strlen(plaintext)<<endl;
//    strcpy(plaintext,aes_de);
//    cout<<"length: "<<strlen(plaintext)<<endl;
    //memcpy(plaintext,aes_de,strlen(aes_de));
    //plaintext=aes_de;

    // bool as=true;
    // printf("%d",as);
    // string  aes_de_str;
    // aes_de_str=aes_de;
    // return aes_de_str;
    return aes_de;


}
