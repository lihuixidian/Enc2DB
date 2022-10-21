#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <iostream>

#include <vector>
#include <string>
#include "string.h"
#include <sstream>
#include <typeinfo>
#include <cstdio>



#include "errors.h"
#include "t.h"


#ifdef __cplusplus
extern "C"
{
#endif
#include "ore_crypto.h"
#include "ore_blk.h"
#include "errors.h"
#include "ORE_STR22STRUCT.h"

#ifdef __cplusplus
}
#endif


int getLen(const unsigned char s[])
{

    int nLen = 0;

    const unsigned char *p = s;

    while (*p != 0)
    {

        nLen++;

        p++;
    }

    return nLen;
}

string ctext_to_str(char **dest, ore_blk_ciphertext cipher)
{
    unsigned int size = 0;
    string str0;
    t change;

    if (dest == NULL || cipher == NULL)
        return NULL;

    // size =  100 * sizeof(int) + 20 *sizeof(char);
    size = 2 * sizeof(bool) + 512;
    vector<byte> vecData_right;
    vector<int> vecData_right_int;

    size_t bufsz1 = sizeof(cipher->params) + 1;
    size_t hexsz1 = 2 * (sizeof(cipher->params) + 1);
    char *h_blk_params = new char[hexsz1 * 2];
    change.buf2hex((uint8_t *)cipher->params, sizeof(cipher->params), h_blk_params, &hexsz1);

    // size_t bufsz3 = getLen(cipher->comp_left) + 1;

    // 设置加密字段的长度
    uint32_t nblocks_left = CEIL(cipher->params->nbits, cipher->params->block_len);

    size_t bufsz3 = (AES_BLOCK_LEN + CEIL(cipher->params->nbits, 8)) * nblocks_left;

    // size_t hexsz3 = 2 * (getLen(cipher->comp_left) + 1);
    size_t hexsz3 = 2 * (bufsz3 + 1);
    char *bufleft = new char[hexsz3 * 2];
    // change.buf2hex((uint8_t *)cipher->comp_left, getLen(cipher->comp_left), bufleft, &hexsz3);

    change.buf2hex((uint8_t *)cipher->comp_left, bufsz3, bufleft, &hexsz3);

    // size_t bufsz4 = getLen(cipher->comp_right) + 1;
    uint32_t block_len = cipher->params->block_len;
    uint32_t nslots = 1 << block_len;
    uint32_t nblocks_right = CEIL(cipher->params->nbits, block_len);
    size_t bufsz4 = AES_BLOCK_LEN + CEIL(nslots, 8) * nblocks_right;

    size_t hexsz4 = 2 * (bufsz4 + 1);
    char *bufright = new char[hexsz4 * 2];
    // change.buf2hex((uint8_t *)cipher->comp_right, getLen(cipher->comp_right), bufright, &hexsz4);
    change.buf2hex((uint8_t *)cipher->comp_right, bufsz4, bufright, &hexsz4);

    *dest = (char *)malloc(size);
    if (*dest == NULL)
        return NULL;

    memset(*dest, 0, size);

    sprintf(*dest,
            "%d~"
            "%d!"
            "%u@"
            "%u*",
            cipher->initialized, cipher->params->initialized, cipher->params->nbits, cipher->params->block_len);

    string str_left(bufleft, strlen(bufleft));
    string str_right(bufright, strlen(bufright));
    string str1 = *dest + str_left + "^" + str_right + "<";

    // cout << str1 << endl;

    delete[] h_blk_params;
    delete[] bufleft;
    delete[] bufright;
    free(*dest);
    return str1;
}
char *strsep(char **stringp, const char *delim)
{
    //判断stringp是否为空,提高程序的容错性
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;
    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;)
    {
        c = *s++;
        spanp = delim;
        do
        {
            if ((sc = *spanp++) == c)
            {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
}
void pushToVec(std::vector<std::string> &obj, const char *param, std::string token)
{
    char *p = (char *)param;
    char *key_point;
    while (p)
    {
        while (key_point = strsep(&p, token.c_str())) //关键字为c或d，它们连续出现了
        {
            //函数遇到token时，key_point指向token返回
            if (*key_point == 0)
                continue;
            else
                break;
        }
        if (key_point)
            obj.push_back(key_point);
    }
}

int ctext_parse(string res, ore_blk_ciphertext cipher)
{
    string test1(res);
    string token1("~!@*^<");
    vector<string> obj1;
    //cipher = (ore_blk_ciphertext *) malloc(sizeof(ore_blk_ciphertext));
    // cipher = malloc(sizeof(*cipher));
    pushToVec(obj1, test1.c_str(), token1);

    if (obj1[0] == "1")
    {
        cipher->initialized = true;
    }
    if (obj1[0] == "0")
    {
        cipher->initialized = false;
    }

    if (obj1[1] == "1")
    {
        cipher->params->initialized = true;
    }
    if (obj1[1] == "0")
    {
        cipher->params->initialized = false;
    }

    cipher->params->nbits = std::stoul(obj1[2]);
    cipher->params->block_len = std::stoul(obj1[3]);

    // edit by chen
    // init_ore_blk_ciphertext(cipher, cipher->params);
    // cipher->comp_left = malloc(8);

    t change;
    size_t bufsz1 = sizeof(obj1[4].data()) + 1;
    size_t hexsz1 = 2 * (strlen(obj1[4].data()) + 1);
    char *nc1 = new char[hexsz1];
    // uint8_t nc1[hexsz1];
    change.hex2buf((uint8_t *)nc1, &hexsz1, obj1[4].data(), hexsz1);

    // memcpy(cipher->comp_left, (byte *)nc1, hexsz1);
    cipher->comp_left = (byte *)nc1;

    size_t bufsz2 = sizeof(obj1[5].data()) + 1;
    size_t hexsz2 = 2 * (strlen(obj1[5].data()) + 1);
    char *nc2 = new char[hexsz2];
    change.hex2buf((uint8_t *)nc2, &hexsz2, obj1[5].data(), hexsz2);

    cipher->comp_right = (byte *)nc2;
    // memcpy(cipher->comp_right, (byte *)nc2, hexsz2);

    // delete []nc1;
    // delete []nc2;

    return ERROR_NONE;
}

int ore_compare(char *strctxt1, char *strctxt2)
{
    ore_blk_ciphertext ctxt1;
    ore_blk_ciphertext ctxt2;
    
    ERR_CHECK(ctext_parse(strctxt1, ctxt1));
    ERR_CHECK(ctext_parse(strctxt2, ctxt2));

    int res;
    ERR_CHECK(ore_blk_compare(&res, ctxt1, ctxt2));

    ERR_CHECK(clear_ore_blk_ciphertext(ctxt1));
    ERR_CHECK(clear_ore_blk_ciphertext(ctxt2));

    return res;

}

// int main(int argc, char *argv[])
// {
//     int nbits = 32;
//     int block_len = 8;

//     uint64_t n1 = 5555555;
//     uint64_t n2 = 5555555;
//     uint64_t n3 = 2345;

//     // uint64_t n1 = rand() % (((uint64_t) 1) << nbits);
//     // uint64_t n2 = rand() % (((uint64_t) 1) << nbits);
//     printf("\n");
//     printf("明文1是： %u\n", n1);
//     printf("明文2是： %u\n", n2);

//     int cmp = (n1 < n2) ? -1 : 1;
//     if (n1 == n2)
//     {
//         cmp = 0;
//     }

//     ore_blk_params params;
//     ERR_CHECK(init_ore_blk_params(params, nbits, block_len));

//     ore_blk_secret_key sk;
//     ERR_CHECK(ore_blk_setup(sk, params));

//     ore_blk_ciphertext ctxt1;
//     ERR_CHECK(init_ore_blk_ciphertext(ctxt1, params));

//     ore_blk_ciphertext ctxt2;
//     ERR_CHECK(init_ore_blk_ciphertext(ctxt2, params));

//     ore_blk_ciphertext ctxt3;
//     ERR_CHECK(init_ore_blk_ciphertext(ctxt3, params));

//     ERR_CHECK(ore_blk_encrypt_ui(ctxt1, sk, n1));
//     ERR_CHECK(ore_blk_encrypt_ui(ctxt2, sk, n2));
//     ERR_CHECK(ore_blk_encrypt_ui(ctxt3, sk, n3));

//     //问题在于com_right的地址发生了改变
//     ore_blk_ciphertext a;
//     ore_blk_ciphertext b;
//     ore_blk_ciphertext *p = NULL;
//     ore_blk_ciphertext cipher;
//     //     //ore_blk_ciphertext &p_1=*p;
//     char *str = NULL;
//     string res;
//     int res1;

//     a->initialized = ctxt1->initialized;
//     a->params->initialized = ctxt1->params->initialized;
//     a->params->nbits = 32;
//     a->params->block_len = 8;
//     a->comp_right = ctxt1->comp_right;
//     a->comp_left = ctxt1->comp_left;

//     res = ctext_to_str(&str, ctxt1);
//     ERR_CHECK(ctext_parse(res, cipher));

//     string strctxt2;
//     char *strctxt2_1 = NULL;
//     // ore_blk_ciphertext cipher2;
//     strctxt2 = ctext_to_str(&strctxt2_1, ctxt2);
//     // ERR_CHECK(ctext_parse(strctxt2, cipher2));

//     cout << "ctxt1的密文: " <<res<<endl;
//     cout << "ctxt2的密文: " <<strctxt2<< endl;


//     int res3;
//     ore_blk_compare(&res3, ctxt1, cipher);
//     cout << "比较结果(ctxt1, cipher)：" << res3 << endl;

//     int res4;
//     ore_blk_compare(&res4, ctxt2, cipher);
//     cout << "比较结果(ctxt2, cipher)：" << res4 << endl;

//     int res5;
//     ore_blk_compare(&res5, ctxt2, ctxt1);
//     cout << "比较结果(ctxt2, ctxt1)：" << res5 << endl;

//     char *charctxt2 = const_cast<char *>(strctxt2.c_str());
//     char *charctxt1 = const_cast<char *>(res.c_str());
//     int res6 = ore_compare(charctxt2, charctxt1);
//     cout << "比较结果(cipher2, cipher1): " << res6 << endl;

//     ERR_CHECK(clear_ore_blk_ciphertext(cipher));
//     ERR_CHECK(clear_ore_blk_ciphertext(ctxt1));
//     ERR_CHECK(clear_ore_blk_ciphertext(ctxt2));
//     ERR_CHECK(clear_ore_blk_ciphertext(ctxt3));
// }
