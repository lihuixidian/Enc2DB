
#pragma once



#ifdef __cplusplus
extern "C" {
#endif

#include "postgres.h"
#include "fmgr.h"
#include "utils/geo_decls.h"

enum OpType {
    T_SAHE,
    T_SMHE,
    T_ORE,
    T_DET
};

struct WarpperParam
{
    void* p1;
    void* p2;
    void* p3;
    void* p4;

    void* func;
    OpType type;
    /* data */
};


char*(*func4p_t)(char*,char*,char*,char*);

text* callFunction4p(text*, text*, text*, text*, func4p_t );

#ifdef __cplusplus
}

#endif

