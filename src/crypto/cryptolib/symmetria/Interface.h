//
// Created by 甜甜 on 2020/12/14.
//

#ifndef SYMMETRIA_2_INTERFACE_H
#define SYMMETRIA_2_INTERFACE_H

//#ifdef __cplusplus
//#include <iostream>
//#include <stdio.h>
//#include <memory.h>
//#include <map>
//#include <hash_map>
//#include <vector>
//#include <string>

//
//#include<ext/hash_map>
// using namespace __gnu_cxx;
// using namespace  std;
//#endif

// class Interface {
// public:
//    str22struct str22;
//    t change;

//#ifdef __cplusplus
// extern "C"{
//#endif
//
//    extern char* SAHE_add_udf(char cipher1[],char cipher2[]);
//
//#ifdef __cplusplus
//}
//#endif
char* SAHE_add_udf(char* cipher1, char* cipher2);
char* SAHE_addplaintext_udf(char* cipher1, int m);
char* SAHE_substract_udf(char* cipher1, char* cipher2);
char* SAHE_multiply_udf(char* cipher1, int m);
char* SAHE_neggate_udf(char* cipher1);
char* SMHE_MULTIPLY_udf(char* cipher1, char* cipher2);
char* SMHE_MULTIPLYPLAIN_udf(char* cipher1, int m);
char* SMHE_DIVIDE_udf(char* cipher1, char* cipher2);
char* SMHE_POW_udf(char* cipher1, int m);
char* SMHE_INVERSE_udf(char* cipher1);

//};
//#ifdef __cplusplus
//}
//#endif
//};

#endif  // SYMMETRIA_2_INTERFACE_H
