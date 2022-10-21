#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "postgres.h" // 与 #include <iomanip> 冲突

#include "utils/palloc.h"
#include "utils/memutils.h"

#include "postgres.h"
#include "nodes/value.h"
#include "nodes/parsenodes.h"
#include "settings.h"
#ifdef __cplusplus
}
#endif

#include <string>
#include "crypto/crypto.h"

using std::string;

typedef enum
{
    // order is important！
    T_STRING_COLUMN,
    T_STRING_TABLE,
    T_STRING_SCHEMA,
    T_STRING_CATALOG,
    T_STRING_UDF_FUNCTION,
    T_STRING_RESTMP,
    T_STRING_TBLTMP,
    T_STRING_COUNT,
    // above type need to be mapped

    // those two need to be encrypt
    T_STRING_NORMAL,
    T_NUMBER,

    // 统一代表上面两个
    T_VALUE,

} PlainDataType;

// CPP.h 一个， C.h 一个。

extern const int Short_Buffer_Size;

int debug2file(const char *sql, bool b);

inline int max(int a, int b)
{
    return a > b ? a : b;
}

inline int min(int a, int b)
{
    return a < b ? a : b;
}

int errmsg(const char *fmt, ...);

int errdebug(const char *fmt, ...);

void cleanup();

char *addEncryptSubfix(T_Cipher t, const char *colname);

T_Cipher getCipherTypeByOp(const char *op, bool);

size_t getValueLength(Value *v);

size_t getStrLength(const char *str);

const char *getAExprOp(A_Expr *expr);

char *getRandomBytesInBase64(size_t mapperBufSize);
char *getRandomBytesInHex(size_t mapperBufSize);

List *removeConstraints(List *constraints, ConstrType t);

bool hasConstraintAndReturn(List *constraints, ConstrType t, Constraint **c);

bool checkCmd(const char *sql, bool *Encrypt);

char *getMappedName(PlainDataType t, const char *k, const char *defaultV);
char *setMappedName(PlainDataType t, const char *k, const char *v);

void getCiphersByTypename(const char *typeName, int *ciphers, int *size); // 需要自行分配内存

/////////////////////////////////////////////////
///// Used in node2str.cpp
////////////////////////////////////////////////

const char *getSortTypeBy(SortByDir dir);

const char *constrType2Str(ConstrType t);

////////////////////////////////////
// decryptres.cpp
////////////////////////////////////

extern const int Short_Buffer_Size;

typedef float FLOAT;
typedef long INTEGER;

typedef struct pgresAttValue
{
    int len;     /* length in bytes of the value */
    char *value; /* actual value, plus terminating zero byte */
} PGresAttValue;

typedef struct pgresAttDesc
{
    char *name;    /* column name */
    Oid tableid;   /* source table, if known */
    int columnid;  /* source column, if known */
    int format;    /* format code for value (text/binary) */
    Oid typid;     /* type id */
    int typlen;    /* type size */
    int atttypmod; /* type-specific modifier info */
} PGresAttDesc;

typedef enum
{
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_STRING
} COLUMN_TYPE;

typedef struct
{
    int len;           // length of ciphers
    char *name;        // name of column
    char *type;        // type of column
    T_Cipher *ciphers; // ciphers of column
} ColumnInfo;          // column的相关信息

COLUMN_TYPE unifyColumnType(const std::string &s);

void IntTypetoString(const char *buf, char *const v, void *others);
void FloatTypetoString(const char *buf, char *const v, void *others);

string ltos(long l);

// Time things
#define UNIX_EPOCH_JDATE 2440588     /* == date2j(1970, 1, 1) */
#define POSTGRES_EPOCH_JDATE 2451545 /* == date2j(2000, 1, 1) */

#define SECS_PER_YEAR (36525 * 864) /* avoid floating-point computation */
#define SECS_PER_DAY 86400
#define SECS_PER_HOUR 3600
#define SECS_PER_MINUTE 60
#define MINS_PER_HOUR 60
#define USECS_PER_SEC INT64CONST(1000000)

long GetCurrentTimestamp(void);
void fmttime(time_t lt1, char *res);