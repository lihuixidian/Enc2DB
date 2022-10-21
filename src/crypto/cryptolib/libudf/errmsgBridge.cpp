
extern "C" {
#include "postgres.h"
}


void errmsgBridge(int level, const char* msg) {
    ereport(level,
            (errcode(ERRCODE_CANNOT_CONNECT_NOW),
             errmsg(msg)));
}