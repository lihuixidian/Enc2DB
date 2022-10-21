

#ifdef __cplusplus
extern "C"
{
#endif

<<<<<<< HEAD:src/encryptsql/utils.cpp
#include "encryptsql/combine.h"
// #include "encryptsql/encryptstmt.h"
#include "encryptsql/node2str.h"
=======
#include "postgres.h" // 与 #include <iomanip> 冲突
>>>>>>> dbdev:src/utils/utils.cpp

#include "utils/palloc.h"
#include "utils/memutils.h"
// #include "encryptsql/encryptstmt.h"
#include "encryptsql/node2str.h"
#include "base64.h"
#include "nodes/nodes.h"
#include "values.h"
#include "nodes/parsenodes.h"
#include "nodes/pg_list.h"

#include "utils/memutils.h"

#include <openssl/crypto.h>

#ifdef __cplusplus
}
#endif
#include "crypto/openssl.h"

#include <string>
#include <vector>
#include <algorithm> // transform
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <time.h>
#include <cmath>

<<<<<<< HEAD:src/encryptsql/utils.cpp
#include "encryptsql/utils.h"
=======
#include "utils.h"
>>>>>>> dbdev:src/utils/utils.cpp
#include "encryptsql/fieldmap.h"

constexpr int Short_Buffer_Size = 64; // column名字buffer，以及decrypt中的buffer

char *getMappedName(PlainDataType t, const char *k, const char *defaultV) // 此函数自动分配内存
{
    char *res = (char *)palloc(Short_Buffer_Size); // WARN
    const char *mapstr = plainDataType2String(t);
    char *defaultV_;
    if (defaultV)
    {
        defaultV_ = (char *)palloc(strlen(defaultV) + 2);
        strcpy(defaultV_, defaultV);
    }
    else
    {
        defaultV_ = (char *)palloc(10);
        strcpy(defaultV_, NOEXIST);
    }
    getValueBy(mapstr, k, res);
    if (!*res && !defaultV)
    {
        errmsg("unable to get mapped name %s: %s\n", mapstr, k);
    }

    return *res ? res : defaultV_;
}

char *setMappedName(PlainDataType t, const char *k, const char *v)
{
    // 添加前缀并且存储到mapper中
    const char *mapstr = plainDataType2String(t);
    char *newV = (char *)palloc(strlen(mapstr) + strlen(v) + 4);
    sprintf(newV, "%s_%s", mapstr, v);
    setValueBy(mapstr, k, newV);
    return newV;
}

int errdebug(const char *fmt, ...)
{
#ifndef NDEBUG
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);

    printf("DEBUG: %s\n", buf);
#endif

    // exit(-1);
    return 0; /* return value does not matter */
}

int errmsg(const char *fmt, ...)
{

    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);

    printf("ERROR: %s\n", buf);

    // exit(-1);
    return 0; /* return value does not matter */
}

int debug2file(const char *sql, bool isSql)
{
    using namespace std;
    ofstream fp("logfile", ofstream::app);
    if (isSql)
        fp << "SQL: " << sql << "\n========================>\n";
    else
        fp << sql << endl;
    fp.close();
    return 0;
}

size_t getValueLength(Value *v)
{
    switch (v->type)
    {
    case T_String:
        return strlen(strVal(v));

    case T_Integer:
        return sizeof(long);
    case T_Float:
        return sizeof(float);
    default:
        break;
    }
    return 0;
}

char *addEncryptSubfix(T_Cipher t, const char *colname)
{
    char *subfix = (char *)typeCipher2String(t);
    char *newColName = (char *)palloc(strlen(colname) + strlen(subfix) + 2);
    sprintf(newColName, "%s%s", colname, subfix);
    return newColName;
}

const char *getAExprOp(A_Expr *expr)
{
    const char *op;
    A_Expr_Kind kind = expr->kind;
    if (kind == AEXPR_AND)
        op = "AND";
    else if (kind == AEXPR_OR)
        op = "OR";
    else if (kind == AEXPR_NOT)
        op = "NOT";
    else if (kind == AEXPR_IN)
        op = "IN";
    else
        op = strVal(linitial(expr->name));
    return op;
}

T_Cipher getCipherTypeByOp(const char *op, bool isRet)
{
    if (!strcmp("+", op) || !strcmp("sum", op))
        return CIPHER_SAHE;
    if (!strcmp("-", op) || !strcmp("sub", op))
        return CIPHER_SAHE;

    if (!strcmp("*", op) || !strcmp("mul", op) || !strcmp("pow", op))
        return CIPHER_SMHE;
    if (!strcmp("/", op) || !strcmp("div", op))
        return CIPHER_SMHE;

    if (!strcmp("=", op) || !strcmp("eq", op) || !strcmp("IN", op))
        return CIPHER_AES;

    if (!strcmp("AND", op) || !strcmp("OR", op))
        return CIPHER_NOCRYPT;

    if (!strcmp("<=", op) || !strcmp(">=", op) || !strcmp("<>", op) || !strcmp("!=", op) || !strcmp("<", op) ||
        !strcmp(">", op) || !strcmp("max", op) || !strcmp("min", op))
<<<<<<< HEAD:src/encryptsql/utils.cpp
        return CIPHER_ORE;
=======
#if defined(SGX_ORE) || defined(SGX_MHE)
        return CIPHER_AES;
#else
        return CIPHER_ORE;
#endif
>>>>>>> dbdev:src/utils/utils.cpp

    if (!strcmp("count", op) && isRet) // count 的返回值不加密， 函数参数要选择加密列
        return CIPHER_NOCRYPT;

    return DEFALUT_ENCRYPT_CIPHER;
}

char *getRandomBytesInBase64(size_t mapperBufSize)
{
    size_t b64Size = BASE64_ENCODE_OUT_SIZE(mapperBufSize);
    char *b64bytes = (char *)palloc(b64Size);
    uint8_t *tmpBytes = (uint8_t *)palloc(mapperBufSize);
    genRandomBytes(tmpBytes, mapperBufSize);
    base64_encode(tmpBytes, mapperBufSize, b64bytes);
    pfree(tmpBytes);
    return b64bytes;
}

char *getRandomBytesInHex(size_t mapperBufSize)
{

    size_t strLen = 2 * mapperBufSize + 1;
    char *bytes = (char *)palloc(strLen);
    uint8_t *tmpBytes = (uint8_t *)palloc(mapperBufSize);
    genRandomBytes(tmpBytes, mapperBufSize);
    buf2hex(tmpBytes, mapperBufSize, bytes, &strLen);
    pfree(tmpBytes);
    return bytes;
}

List *removeConstraints(List *constraints, ConstrType t)
{
    ListCell *cell;
    foreach (cell, constraints)
    {
        Constraint *constraint = (Constraint *)lfirst(cell);
        if (constraint->contype == t)
            constraints = list_delete(constraints, constraint);
    }
    return constraints;
}

bool hasConstraintAndReturn(List *constraints, ConstrType t, Constraint **c)
{
    ListCell *cell;
    foreach (cell, constraints)
    {
        if (((Constraint *)lfirst(cell))->contype == t)
        {
            *c = (Constraint *)lfirst(cell);
            return true;
        }
    }
    return false;
}

static void CppSplitString(const std::string &s, std::vector<std::string> &v, const std::string &c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

bool checkCmd(const char *sql_, bool *Encrypt)
{
    using namespace std;
    vector<string> v;
    string sql = sql_;

    if (sql.back() == ';')
        sql.pop_back();
    CppSplitString(sql, v, " ");
    if (v.size() != 2)
        return false;
    if (v[1].back() < 97 || v[1].back() > 122)
        v[1].pop_back();
    for (auto &it : v)
        transform(it.begin(), it.end(), it.begin(), ::tolower);

    if (!v[0].compare("encrypt") || !v[0].compare("enc"))
    {
        if (!v[1].compare("on"))
            *Encrypt = true;
        else if (!v[1].compare("off"))
            *Encrypt = false;
        else if (!v[1].compare("status"))
            return true;
    }
    else
        return false;

    return true;
}

/////////////////////////////////////////////////
///// Used in node2str.cpp
////////////////////////////////////////////////

const char *getSortTypeBy(SortByDir dir)
{
    switch (dir)
    {
    case SORTBY_ASC:
        return "ASC";

    case SORTBY_DESC:
        return "DESC";

    case SORTBY_USING:
        return "USING";
    default:
        return "";
    }
    return "";
}

const char *constrType2Str(ConstrType t)
{
    const char *ans;
    switch (t)
    {
    case CONSTR_NULL:
        ans = "NULL";
        break;
    case CONSTR_NOTNULL:
        ans = "NOT NULL";
        break;
    case CONSTR_DEFAULT:
        ans = "DEFAULT";
        break;
    case CONSTR_CHECK:
        ans = "CHECK";
        break;
    case CONSTR_PRIMARY:
        ans = "PRIMARY KEY";
        break;

    case CONSTR_UNIQUE:
        ans = "UNIQUE";
        break;
    case CONSTR_EXCLUSION:
        ans = "EXCLUSION";
        break;

    case CONSTR_FOREIGN:
        ans = "FOREIGN KEY";
        break;

    default:
        errmsg("unhandled const type: %d in constrType2Str()\n", t);
        break;
        // return ans;
    }
    return ans;
}

void cleanup()
{
    mapperCleanup();
}

////////////////////////////////////////////////////
/// Used in decryptres.cpp
///////////////////////////////////////////////////

COLUMN_TYPE unifyColumnType(const std::string &s) // 已经解析完的string都是小写
{
    if (s.find("int") != s.npos || s.find("serial") != s.npos || !s.compare("timestamp"))
        return TYPE_INTEGER;
    if (s.find("numeric") != s.npos || s.find("real") != s.npos || s.find("double") != s.npos ||
<<<<<<< HEAD:src/encryptsql/utils.cpp
        s.find("decimal") != s.npos) // WARN 序列数据多个列
=======
        s.find("decimal") != s.npos || s.find("float") != s.npos) // WARN 序列数据多个列
>>>>>>> dbdev:src/utils/utils.cpp
        return TYPE_FLOAT;

    return TYPE_STRING;
}

void IntTypetoString(const char *buf, char *const v, void *others)
{
    long l = *(long *)buf;
<<<<<<< HEAD:src/encryptsql/utils.cpp

    double f = (l / Float_Scale);

    if (others && !strcmp((char *)others, "timestamp"))
    { //用long表示的时间格式
        fmttime((time_t)f, (char *)v);
=======

    // double f = (l / 1);

    if (others && !strcmp((char *)others, "timestamp"))
    { //用long表示的时间格式
        fmttime((time_t)l, (char *)v);
>>>>>>> dbdev:src/utils/utils.cpp
        return;
    }

    std::ostringstream ss;
    ss << l;
    strcpy(v, ss.str().c_str());
}

<<<<<<< HEAD:src/encryptsql/utils.cpp
void FloatTypetoString(const char *buf, char *const v, void *others) // TODO: // buf 不一定是 double的内存排布，还有可能是long， 因为词法分析不区分 (long)1, (double)1
{
    long l = *(long *)buf;
    double f = (l / Float_Scale) * 1.0;

    std::ostringstream ss;
    ss << f;
    strcpy(v, ss.str().c_str());
=======
void FloatTypetoString(const char *buf, char *v, void *others) // TODO: // buf 不一定是 double的内存排布，还有可能是long， 因为词法分析不区分 (long)1, (double)1
{
    long l = *(long *)buf;
    double f = (l / Float_Scale) * 1.0;
    sprintf(v, "%.10f", f);
    size_t sz = strlen(v);
    char *c = NULL;
    // 去掉小数点后多余的0
    for(c = v; *c && *c != '.'; ++c) {
    }
    if(!*c) {// 没有小数点，说明不需要清除
        return ;
    }

    for (c = (v + sz - 1); *c == '0' || *c == '.'; --c) {
        if(c == v) {
            break;
        }
        
        if(*c == '.') {
            *c = '\0';
            break;
        }
        *c = '\0';
        
    }
    

>>>>>>> dbdev:src/utils/utils.cpp
}

void getCiphersByTypename(const char *typeName, int *ciphers, int *size) // 需要自行分配内存
{
<<<<<<< HEAD:src/encryptsql/utils.cpp
=======
    // TODO: remove DET
>>>>>>> dbdev:src/utils/utils.cpp
    if (!strcmp("serial", typeName))
    { // 序列数据，只做id标识用
        ciphers[0] = CIPHER_NOCRYPT;
        *size = 1;
        return;
    }

    // Order is important!

    // ciphers[0] = CIPHER_SAHE;
    // ciphers[1] = CIPHER_SMHE;
    // ciphers[2] = CIPHER_DET;
    // ciphers[3] = CIPHER_ORE;
    // ciphers[4] = CIPHER_AES;
    // ciphers[5] = CIPHER_NOCRYPT;

    auto type = unifyColumnType(typeName);

    if (type == TYPE_STRING)
    {
<<<<<<< HEAD:src/encryptsql/utils.cpp
        ciphers[0] = CIPHER_DET;
        ciphers[1] = CIPHER_ORE;
        ciphers[2] = CIPHER_AES;
        *size = 3;
=======
#if defined(SGX_ORE) || defined(SGX_MHE)
        ciphers[0] = CIPHER_AES;
        *size = 1;
#else
        // ciphers[0] = CIPHER_DET;
        ciphers[0] = CIPHER_ORE;
        ciphers[1] = CIPHER_AES;
        *size = 2;
#endif

>>>>>>> dbdev:src/utils/utils.cpp
        return;
    }
    else if (type == TYPE_FLOAT || type == TYPE_INTEGER)
    {

#if defined(SGX_ORE) || defined(SGX_MHE)
        ciphers[0] = CIPHER_SAHE;
        ciphers[1] = CIPHER_SMHE;
        // ciphers[2] = CIPHER_DET;
        ciphers[2] = CIPHER_AES;
        *size = 3;
#else
        ciphers[0] = CIPHER_SAHE;
        ciphers[1] = CIPHER_SMHE;
        // ciphers[2] = CIPHER_DET;
        ciphers[2] = CIPHER_ORE;
        ciphers[3] = CIPHER_AES;
        *size = 4;
#endif
        return;
    }
    // return unifyColumnType();
}

// template void numbericTypetoString<FLOAT>(const char*, char* const,);
// template void numbericTypetoString<INTEGER>(const char*, char* const);

////////////////////////////////////////////////////
/// Used in fieldmap.cpp
///////////////////////////////////////////////////

<<<<<<< HEAD:src/encryptsql/utils.cpp
ColumnInfo *makeColumnInfo()
{
    ColumnInfo *cinfo = (ColumnInfo *)palloc(sizeof(ColumnInfo));
    // cinfo->ciphers = (T_cipher*)palloc();
}
=======
// ColumnInfo *makeColumnInfo()
// {
//     ColumnInfo *cinfo = (ColumnInfo *)palloc(sizeof(ColumnInfo));
//     // cinfo->ciphers = (T_cipher*)palloc();
// }
>>>>>>> dbdev:src/utils/utils.cpp

string ltos(long l)
{
    using namespace std;
    ostringstream os;
    os << l;
    string result;
    istringstream is(os.str());
    is >> result;
    return result;
}

// Time things

long GetCurrentTimestamp(void)
{
    long result;
    struct timeval tp;

    gettimeofday(&tp, NULL);

    // time_t t;
    // time(&t);

    result = (long)tp.tv_sec;
    result = result + (tp.tv_usec / 1000000.0);

    return result;
}

void fmttime(time_t lt1, char *res)
{

    struct tm *newtime;

    newtime = localtime(&lt1);

    strftime(res, 34, "%F %T", newtime);
}