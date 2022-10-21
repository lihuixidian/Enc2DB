#pragma once

typedef void *pAttrDescs;
typedef void **pTuples;

extern "C" const char *encryptOneSql(const char *sql);

extern "C" void decryptResult(int numberAttr, int numTuples, pAttrDescs pattDescs, pTuples ptuples);
<<<<<<< HEAD
=======

struct EncryptInfo
{
    const char *sql; // 正在处理的sql
    bool isFloatCol; //当前处理的col是否是float
    void* father;
    bool isPeerColFloat;    // where col_float = 10;  这种where条件中，10的AES需要放缩， isPeerColFloat为true表示在一个二元操作符中，操作数为float column.
    bool isALeftOps;
    bool isARightOps;
};
>>>>>>> dbdev
