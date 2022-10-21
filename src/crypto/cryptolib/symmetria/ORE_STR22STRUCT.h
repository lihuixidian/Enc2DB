//
// Created by 甜甜 on 2020/12/26.
//

//
// Created by 甜甜 on 2020/12/13.
//

#ifndef ORE_STR22STRUCT_H
#define ORE_STR22STRUCT_H

/*
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <vector>
#include <string>
#include <sstream>
//#include "t.h"

#include "module.h"
#include "errors.h"
#include "t.h"
*/

//#include "split.h"
// extern "C" {
// #endif
//     #include "ORE_STR22STRUCT.h"
// #ifdef __cplusplus
// }
// #endif
#ifdef  __cplusplus
extern "C" {
#endif
#include "ore_crypto.h"
#include "ore_blk.h"
#include "errors.h"
    

#ifdef __cplusplus
}
#endif
// using namespace std;

typedef unsigned char byte;

#define CEIL(x, y) (((x) + (y)-1) / (y))

// using namespace std;

static int _error;

#define ERR_CHECK(x)                \
    if ((_error = x) != ERROR_NONE) \
    {                               \
        return _error;              \
    }

    ////the public parameters for the encryption scheme, used to compare ciphertexts
    // typedef struct {
    //     bool initialized;    // whether or not these parameters have been initialized
    //     uint32_t nbits;      // the number of bits in the plaintext elements
    //     uint32_t block_len;  // the number of bits in each block of the plaintext
    // }ore_blk_params, ore_blk_ciphertext[1];

    // typedef struct {
    //     bool initialized;    // whether or not these parameters have been initialized
    //     uint32_t nbits;      // the number of bits in the plaintext elements
    //     uint32_t block_len;  // the number of bits in each block of the plaintext
    // }ore_blk_ciphertext[1];
    // // the ciphertexts of the encryption scheme
    // typedef struct {
    //     bool initialized;            // whether or not the ciphertext has been initialized
    //     byte*           comp_left;   // the left ciphertext
    //     byte*           comp_right;  // the right ciphertext
    //     ore_blk_ciphertext  params;
    // } ore_blk_ciphertext[1];

    // string getString(const int& a);
    // string getString(const long long & a);
    // string getString(const string& s);


    // string ctext_to_str(char **dest, ore_blk_ciphertext cipher);
    // char *strsep(char **stringp, const char *delim);
    // void pushToVec(std::vector<std::string>&obj,const char *param,std::string token);
    // vector<string> split(string s, char delim);
    // int ctext_parse(string str, ore_blk_ciphertext *&cipher);
    // //ore_blk_ciphertext *& ctext_parse1(string str, ore_blk_ciphertext *cipher);
    // int readCharToLL(unsigned char* str, int len);
    int ore_compare(char *strctxt1, char *strctxt2);

#endif
