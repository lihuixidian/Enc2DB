//
// Created by 甜甜 on 2020/12/16.
//

#ifndef SYMMETRIA_2_T_H
#define SYMMETRIA_2_T_H
#include <iostream>
// #include <iomanip>
#include <cstring>
#include <sstream>

class t {
    public:
    int buf2hex(const uint8_t* buf, size_t bufSize, char *hexstr, size_t* hexstrlen);
    int hex2buf(uint8_t* buf, size_t* bufSize, const char* strhex, size_t hexstrlen);
    void testbuf2hex(char *c);
    void testBufConversion();

};


#endif //SYMMETRIA_2_T_H
