//
// Created by 甜甜 on 2020/12/16.
//


#include "t.h"


#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

//#ifdef __cplusplus
//extern "C" {
//#endif
//

//#ifdef __cplusplus
//}
//#endif


int t::buf2hex(const uint8_t* buf, size_t bufSize, char *hexstr, size_t* hexstrlen) { // need to alloc hexstr by yourself

    if(*hexstrlen < bufSize * 2)
        return -1;

    stringstream ss;

    for(int i=0; i<bufSize; ++i)
        ss << setfill('0') << setw(2) << hex << (0xff & (unsigned char)buf[i]);
    string mystr = ss.str() + '\0';
    strcpy(hexstr, mystr.c_str());
    *hexstrlen = mystr.size();
    return 0;
}

int t::hex2buf(uint8_t* buf, size_t* bufSize, const char* strhex, size_t hexstrlen)  //  // need to alloc buf by yourself
{
    if (*bufSize < hexstrlen / 2)
        return -1;

    for (unsigned int i = 0; i < hexstrlen; i += 2) {
        std::string byteString(strhex + i, strhex + i + 2);
        uint8_t byte = (uint8_t)strtol(byteString.c_str(), NULL, 16);
        buf[i / 2] = byte;
    }

    *bufSize = hexstrlen / 2;

    return 0;
}

//void testbuf2hex(char *c) __attribute__((visibility("default")));
//void testbuf2hex(char c[])
//{
////    char c[] = "7036114793006788382~1!2@1*0(0)0+0<#NU$LL%LL^";
//    size_t bufsz = sizeof(c) + 1;
//    size_t hexsz = 2 * (sizeof(c) + 1);
//    char* h = new char[hexsz * 2];
//
//    //char* nc = new char[hexsz];
//
//    buf2hex((uint8_t*)c, strlen(c), h, &hexsz);
//    //hex2buf((uint8_t*)nc, &hexsz, h, hexsz);
//    cout << "raw buf:" << c << endl << "hex string:" << h << endl;
//    //cout << "raw buf:" << c << endl << "hex string:" << h << endl << "convted c: "  << nc << endl;
//}
//void testhex2buf(char h[])
//{
//    size_t bufsz = sizeof(h) /2;
//    size_t hexsz =  sizeof(h)/2;
//    char* nc = new char[hexsz];
//    hex2buf((uint8_t*)nc, &hexsz, h, hexsz);
//    cout << "raw hex:" << h << endl << "buf string:" << nc << endl;
//}
void t::testBufConversion()
{
    char c[] = "8310527297162948723~1!2@1*0(0)0+0<#NU$LL%LL^";
    char a[]="333832393436313438333333393338303836377e32213140312a30283029302b303c313c234e55244c4c254c4c5e";
    size_t bufsz = sizeof(c) + 1;
    size_t hexsz = 2 * (sizeof(c) + 1);
    char* h = new char[hexsz * 2];

    //char* nc = new char[hexsz];
//    char* nc2 = new char[hexsz*2];


    buf2hex((uint8_t*)c, strlen(c), h, &hexsz);
    size_t bufsz2 = sizeof(a) + 1;
    size_t hexsz2 = 2 * (sizeof(a) + 1);
    char* nc2 = new char[hexsz2];
    hex2buf((uint8_t*)nc2, &hexsz2, a, hexsz2);
    //hex2buf((uint8_t*)nc2, &hexsz, a, hexsz);

    cout << "raw buf:" << c << endl << "hex string:" << h << endl ;
    cout << "raw buf:" << a << endl << "convted nc: "  << nc2 << endl;
//    <<"convted nc2: "  << nc2 << endl
}
//int main()
//{
//    t chan;
//    char  c[]="7036114793006788382~1!2@1*0(0)0+0<#NU$LL%LL^";
//    //testbuf2hex(c);
//    char h[]="333832393436313438333333393338303836377e32213140312a30283029302b303c313c234e55244c4c254c4c5e";
//    //testhex2buf(h);
//    chan.testBufConversion();
//
//}
