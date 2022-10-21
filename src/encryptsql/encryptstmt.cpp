

#ifdef __cplusplus
extern "C"
{
#endif

<<<<<<< HEAD
#include "encryptsql/combine.h"
=======
#include "postgres.h" // 与 #include <iomanip> 冲突

#include "utils/palloc.h"
#include "utils/memutils.h"

>>>>>>> dbdev
#include "nodes/nodes.h"
#include "nodes/parsenodes.h"
#include "nodes/value.h"

<<<<<<< HEAD
#include "crypto/base64.h"
=======
#include "base64.h"
>>>>>>> dbdev

#include <string.h>

#ifdef __cplusplus
}
#endif

#include "encryptsql/fieldmap.h"
<<<<<<< HEAD

=======
#include <assert.h>
>>>>>>> dbdev
#include <set>
#include <vector>

#include "crypto/crypto.h"
#include "crypto/openssl.h"
#include "encryptsql.h"

<<<<<<< HEAD
static List *encryptValuesLists(List *valuesLists, List *cols);

static char *encryptRVField(PlainDataType p_type, bool *isCreate, char *name);

static CaseExpr *encryptCaseExpr(CaseExpr *ce);

static Node *encryptDispatcher(Node *expr, T_Cipher t);

static JoinExpr *encryptJoinExpr(JoinExpr *joinExpr);

static RangeVar *encryptRangeVar(RangeVar *rv, bool *isCreate);

static A_Const * // finished version 1
encryptAConst(A_Const *aconst, T_Cipher encryptCipher);

static ColumnRef * // finished version 1
encryptColumnRef(ColumnRef *cref, T_Cipher encryptCipher);

static Node *encryptExpr(Node *expr, T_Cipher t);

static Node *encryptAExpr(A_Expr *expr);

static Alias *encryptAlias(Alias *alias);

static RangeSubselect *encryptRangeSubselect(RangeSubselect *rss);

static List *encryptFunctionArgs(List *args, const char *funcName);

static List *encryptFunctionName(List *funcNames);

static FuncCall *encryptFuncCall(FuncCall *func);

static SubLink *encryptSubLink(SubLink *sl);

static List *encryptGroupClause(List *groupClause);

static List *encryptExtendTargetList(List *targetList);

static List *encryptTargetList(List *targetlist, bool);

static List *encryptFromClause(List *fromClause);

static List *encryptSortClause(List *SortClause);

static List *encryptColumnDefToList(ColumnDef *n, const char *_typename);

static List *encrypttableElts(List *tableElts);

static List *encryptList(List *list);

static List *encryptStar();

static void replaceColumnDefType(TypeName *_typename, const char *type, T_Cipher t);

static Constraint *encryptExplicitConstraint(Constraint *con);

static T_Cipher getEncryptTypeFromNode(Node *n); //根据n是表达式或者func来获取要加密的cipher。

static InsertStmt *encryptInsertStmt(InsertStmt *parseTree);
static DeleteStmt *encryptDeleteStmt(DeleteStmt *parseTree);
static UpdateStmt *encryptUpdateStmt(UpdateStmt *parseTree);
static SelectStmt *encryptSelectStmt(SelectStmt *parseTree, List *cols);
static CreateStmt *encryptCreateStmt(CreateStmt *parseTree);
static IndexStmt *encryptIndexStmt(IndexStmt *parseTree);
static AlterTableStmt *encryptAlterTableStmt(AlterTableStmt *parseTree);

static DropStmt *encryptDropStmt(DropStmt *parseTree);

Node *encryptStmt(Node *parseTree);
=======
static List *encryptValuesLists(List *valuesLists, List *cols, EncryptInfo *info);

static char *encryptRVField(PlainDataType p_type, bool *isCreate, char *name, EncryptInfo *info=nullptr);

static CaseExpr *encryptCaseExpr(CaseExpr *ce, EncryptInfo *info);

static Node *encryptDispatcher(Node *expr, T_Cipher t, EncryptInfo *info);

static JoinExpr *encryptJoinExpr(JoinExpr *joinExpr, EncryptInfo *info=nullptr);

static RangeVar *encryptRangeVar(RangeVar *rv, bool *isCreate, EncryptInfo *info);

static A_Const * // finished version 1
encryptAConst(A_Const *aconst, T_Cipher encryptCipher, EncryptInfo *info);

static ColumnRef * // finished version 1
encryptColumnRef(ColumnRef *cref, T_Cipher encryptCipher, EncryptInfo *info);

static Node *encryptExpr(Node *expr, T_Cipher t, EncryptInfo *info);

static Node *encryptAExpr(A_Expr *expr, EncryptInfo *info);

static Alias *encryptAlias(Alias *alias, EncryptInfo *info=nullptr);

static RangeSubselect *encryptRangeSubselect(RangeSubselect *rss, EncryptInfo *info = nullptr);

static List *encryptFunctionArgs(List *args, const char *funcName, EncryptInfo *info);

static List *encryptFunctionName(List *funcNames, EncryptInfo *info);

static FuncCall *encryptFuncCall(FuncCall *func, EncryptInfo *info);

static SubLink *encryptSubLink(SubLink *sl, EncryptInfo *info);

static List *encryptGroupClause(List *groupClause, EncryptInfo *info);

static List *encryptExtendTargetList(List *targetList, EncryptInfo *info);

static List *encryptTargetList(List *targetlist, bool, EncryptInfo *info);

static List *encryptFromClause(List *fromClause, EncryptInfo *info);

static List *encryptSortClause(List *SortClause, EncryptInfo *info);

static List *encryptColumnDefToList(ColumnDef *n, const char *_typename, EncryptInfo *info = nullptr);

static List *encrypttableElts(List *tableElts, EncryptInfo *info);

static List *encryptList(List *list, EncryptInfo *info);

static List *encryptStar(EncryptInfo *info);

static void replaceColumnDefType(TypeName *_typename, const char *type, T_Cipher t, EncryptInfo *info = nullptr);

static Constraint *encryptExplicitConstraint(Constraint *con, EncryptInfo *info);

static T_Cipher getEncryptTypeFromNode(Node *n, EncryptInfo *info=nullptr); //根据n是表达式或者func来获取要加密的cipher。
static Node *dealWithSpecialFunction(FuncCall *func, EncryptInfo *info);

static InsertStmt *encryptInsertStmt(InsertStmt *parseTree, EncryptInfo *info);
static DeleteStmt *encryptDeleteStmt(DeleteStmt *parseTree, EncryptInfo *info);
static UpdateStmt *encryptUpdateStmt(UpdateStmt *parseTree, EncryptInfo *info);
static SelectStmt *encryptSelectStmt(SelectStmt *parseTree, List *cols, EncryptInfo *info);
static CreateStmt *encryptCreateStmt(CreateStmt *parseTree, EncryptInfo *info);
static IndexStmt *encryptIndexStmt(IndexStmt *parseTree, EncryptInfo *info);
static AlterTableStmt *encryptAlterTableStmt(AlterTableStmt *parseTree, EncryptInfo *info);
static ExplainStmt *encryptExplainStmt(ExplainStmt *parseTree, EncryptInfo *info);
static List *encryptAlterTableCmds(List *cmdList, EncryptInfo *info);

static DropStmt *encryptDropStmt(DropStmt *parseTree, EncryptInfo *info);

Node *encryptStmt(Node *parseTree, EncryptInfo *info);
>>>>>>> dbdev

////////////////////////////////////////
//// Problems
///////////////////////////////////////

// select pid, tid from persons, teacher WHERE pid=tid; 从语法树上无法区分
// pid属于哪一个表,当前的实现是将from子句中的表都存下来，然后在每个表中查看是否有这个列;

<<<<<<< HEAD
static A_Const *encryptAConst(A_Const *aconst, T_Cipher encryptCipher)
{
    uint8_t *plainText;
=======
static A_Const *encryptAConst(A_Const *aconst, T_Cipher encryptCipher, EncryptInfo *info)
{
    A_Const *newConst = nullptr;
    if (encryptCipher == CIPHER_NOCRYPT)
    {
        newConst = (A_Const *)copyObject(aconst);
        return newConst;
    }

    uint8_t *plainText = nullptr;
>>>>>>> dbdev
    Node *AConstValue = (Node *)&(aconst->val);
    size_t out_size = 0;
    size_t in_size = 0;
    uint8_t *cipherText = nullptr;
<<<<<<< HEAD

    A_Const *newConst = makeNode(A_Const);

=======
    bool isFloat = info->isFloatCol;
    newConst = makeNode(A_Const);
    // bool isFloatCol = false;
>>>>>>> dbdev
    if (IsA(AConstValue, Null))
    {
        newConst = (A_Const *)copyObject((void *)AConstValue);
        return newConst;
    }

<<<<<<< HEAD
    if (IsA(AConstValue, Integer))
    {

        auto tmpInt = (long *)palloc(sizeof(long));
        *tmpInt = intVal(AConstValue) * Float_Scale;
        plainText = (uint8_t *)tmpInt;
        in_size = sizeof(long);
=======
    if (IsA(AConstValue, String))
    {
        plainText = (uint8_t *)strVal(AConstValue);
        in_size = strlen((char *)plainText);
>>>>>>> dbdev
    }
    else
    {
<<<<<<< HEAD

        auto tmpInt = (long *)palloc(sizeof(long));
        double tmpDouble = atof(strVal(AConstValue));
        *tmpInt = tmpDouble * Float_Scale;
        // *tmpInt = static_cast<long>(atof(strVal(AConstValue)));
=======
        auto tmpInt = (int64_t *)palloc(sizeof(int64_t));
        double tmpDouble;
        if (IsA(AConstValue, Float) || (info->isPeerColFloat)) // 
        {
            isFloat = true;
            tmpDouble = atof(strVal(AConstValue));
            *tmpInt = tmpDouble * Float_Scale;
            if(info->isPeerColFloat) {
                info->isPeerColFloat = false;
            }
        }
        else
        {
            *tmpInt = intVal(AConstValue);
            if (info->isFloatCol)
            { // 当前列是float列
                *tmpInt *= Float_Scale;
            }
        }

>>>>>>> dbdev
        plainText = (uint8_t *)tmpInt;
        in_size = sizeof(int64_t);
    }

<<<<<<< HEAD
    else
    {
        plainText = (uint8_t *)strVal(AConstValue);
        in_size = strlen((char *)plainText);
    }
    cipherText = encryptValue(encryptCipher, plainText, in_size, &out_size);

    // if(encryptCipher == CIPHER_SAHE || encyr)
    if (encryptCipher == CIPHER_ORE)
    {
=======
    cipherText = encryptValue(encryptCipher, plainText, in_size, &out_size, true, isFloat);

#if defined(SGX_ORE) || defined(SGX_MHE)
    if (encryptCipher == CIPHER_SAHE || encryptCipher == CIPHER_SMHE)
#else
    if (encryptCipher == CIPHER_ORE || encryptCipher == CIPHER_SAHE || encryptCipher == CIPHER_SMHE)
#endif
    {
        // 不需要 buf2hex
>>>>>>> dbdev
        newConst->val = *makeString((char *)cipherText);
    }
    else if (encryptCipher == CIPHER_NOCRYPT)
    {
        newConst->val = *makeInteger(*(int64_t *)cipherText);
    }
    else
    {
        size_t bufEncodingStrSz = out_size * 2 + 1;
        uint8_t *bufEncodingStr = (uint8_t *)palloc0(bufEncodingStrSz);
        buf2hex(cipherText, out_size, (char *)bufEncodingStr, &bufEncodingStrSz);
        newConst->val = *makeString((char *)bufEncodingStr);
    }

    return newConst;
}

<<<<<<< HEAD
static List *encryptStar()
=======
static List *encryptStar(EncryptInfo *info)
>>>>>>> dbdev
{ // 处理from表的第一个匹配项

    char **cols;
    int nColumn = 0;
    List *resList = NIL;
    resList = lappend(resList, makeNode(A_Star));
    getFirstTableColumnsAlloc(&cols, &nColumn);

    for (size_t i = 0; i < nColumn; i++)
    {

        char *plainColname = addEncryptSubfix(DEFALUT_STAR_EXPR_CIPHER, cols[i]);
        char *cipherColname = getMappedName(T_STRING_COLUMN, plainColname, "");

        if (!*cipherColname) // 此列没有默认要求的加密列，可能是serial数据类型，这种数据类型默认不加密。
        {
            plainColname = addEncryptSubfix(CIPHER_NOCRYPT, cols[i]);
            cipherColname = getMappedName(T_STRING_COLUMN, plainColname, NULL);
        }

        resList = lappend(resList, makeString(cipherColname));
    }

    return resList;
}

<<<<<<< HEAD
static ColumnRef *encryptColumnRef(ColumnRef *cref, T_Cipher encryptCipher)
{
=======
static ColumnRef *encryptColumnRef(ColumnRef *cref, T_Cipher encryptCipher, EncryptInfo *info)
{
    
>>>>>>> dbdev
    char *name = NULL;
    char *patchedName = NULL;
    ColumnRef *newCref = (ColumnRef *)copyObject(cref);
    ListCell *p = list_head(cref->fields);
    int listSize = list_length(cref->fields);

    newCref->fields = NIL;

    for (int i = 0; i < listSize; ++i)
    {
        char *cipherColName;
        Value *cipherColNameValue;
        PlainDataType t = (PlainDataType)(listSize - 1 - i);
        // deal with '*'
        if (IsA(lfirst(p), A_Star))
        {
            // newCref->fields = lappend(newCref->fields, makeNode(A_Star));
            newCref->fields = encryptStar(info);
            break;
        }
        name = strVal(lfirst(p));

        if (t == T_STRING_TABLE)
            setCurrentNodeOnce(name, "");

        else if (t == T_STRING_SCHEMA)
            setCurrentNodeOnce("", name);

        else if (t == T_STRING_COLUMN)
        {
            patchedName = addEncryptSubfix(encryptCipher, name);
        }
        patchedName = patchedName ? patchedName : name;
        cipherColName = getMappedName(
            t, patchedName, name);
<<<<<<< HEAD
=======
        if (strcmp(name, cipherColName) == 0)
        { // 这个字段没有encryptCipher， 可能是个NOCRYPT加密
            patchedName = addEncryptSubfix(CIPHER_NOCRYPT, name);
            cipherColName = getMappedName(
                t, patchedName, nullptr);
        }
>>>>>>> dbdev
        cipherColNameValue = makeString((char *)cipherColName);
        newCref->fields = lappend(newCref->fields, cipherColNameValue);
        p = lnext(p);
        char typebuf[128];
        if(IsA((Node*)info->father, A_Expr)) {
            getColumnType(name, typebuf);
            if(!strcmp(typebuf, "float")) {
                info->isPeerColFloat = true;
            }
        }
    }
    return newCref;
}

<<<<<<< HEAD
static SubLink *encryptSubLink(SubLink *sl)
=======
static SubLink *encryptSubLink(SubLink *sl, EncryptInfo *info)
>>>>>>> dbdev
{
    SubLink *newSl = makeNode(SubLink);
    *newSl = *sl;
    if (sl->subLinkType == EXPR_SUBLINK || sl->subLinkType == EXISTS_SUBLINK)
    {
        if (sl->subselect)
<<<<<<< HEAD
            newSl->subselect = (Node *)encryptSelectStmt((SelectStmt *)sl->subselect, NULL);
=======
            newSl->subselect = (Node *)encryptSelectStmt((SelectStmt *)sl->subselect, NULL, info);
>>>>>>> dbdev
    }
    else
    {
        errmsg("unhandled sublink type: %d in encryptSubLink()", (int)EXPR_SUBLINK);
    }
    return newSl;
}

<<<<<<< HEAD
static Node *encryptExpr(Node *expr, T_Cipher t)
=======
static Node *encryptExpr(Node *expr, T_Cipher t, EncryptInfo *info)
{
    return encryptDispatcher(expr, t, info);
}

static bool isTransformToFunction(const string &name)
>>>>>>> dbdev
{
    if (strcmp(name.c_str(), "=") == 0) // 赋值语句不需要转换
        return false;
    if (strcmp(name.c_str(), "AND") == 0 || strcmp(name.c_str(), "OR") == 0) // 赋值语句不需要转换
        return false;
    return true;
}

<<<<<<< HEAD
static Node *encryptAExpr(A_Expr *expr)
{
    Node *ans;
    const char *op = getAExprOp(expr);
    T_Cipher t = getCipherTypeByOp(op, false);
    Node *encLexpr = encryptExpr(expr->lexpr, t);
    Node *encRexpr = encryptExpr(expr->rexpr, t);

    if (t != CIPHER_NOCRYPT && t != CIPHER_DET)
=======
static Node *encryptAExpr(A_Expr *expr, EncryptInfo *info) // 将表达式转为 UDF. TODO: 可以提供一个参数，意为这个UDF的结果的加密形式
{
    info->father = (void*)expr;
    Node *ans;
    const char *op = getAExprOp(expr);
    T_Cipher t1 = getCipherTypeByOp(op, false);
    T_Cipher t2 = t1;
    bool hasSecondCipherParams = true; // 第二个参数是否是密文， pow函数第二个不是密文

    if (strcmp(op, "pow") == 0)
    {
        t2 = CIPHER_NOCRYPT;
        hasSecondCipherParams = false;
    }
    Node *encLexpr = nullptr;
    Node *encRexpr = nullptr;

    //!ORE 将ORE换成AES
    if (t1 == CIPHER_ORE)
    {
//不强制使用SGX计算
#if !defined(SGX_ORE) && !defined(SGX_SMHE)
        encLexpr = encryptExpr(expr->lexpr, t1, info);
        encRexpr = encryptExpr(expr->rexpr, t2, info);
#endif
    }
    else if (t1 == CIPHER_SMHE)
    {
#if !defined(SGX_SMHE)
        encLexpr = encryptExpr(expr->lexpr, t1, info);
        encRexpr = encryptExpr(expr->rexpr, t2, info);
#endif
    }
    else
    {
        if (t1 != CIPHER_AES)
            encLexpr = encryptExpr(expr->lexpr, t1, info);
        if (t2 != CIPHER_AES)
            encRexpr = encryptExpr(expr->rexpr, t2, info);
    }

    if (isTransformToFunction(op)) // SAHE SMHE ORE need to be calced in UDF
>>>>>>> dbdev
    {
        FuncCall *funcCall = makeNode(FuncCall);
        char *newFuncName = getMappedName(T_STRING_UDF_FUNCTION, op, NULL);
        funcCall->funcname = lappend(NIL, makeString(newFuncName));
        // 如果定义了SGX_ORE则 ore_udf(col1_aes, col2_aes)
        // 如果没有定义 则为 ore_udf(col1_ore,col2_ore, col1_aes, col2_aes)
        // 类比于MHE
        if (!encLexpr)
            encLexpr = (Node *)makeString("null");
        if (!encRexpr)
        {
            if (hasSecondCipherParams)
                encRexpr = (Node *)makeString("null");
            else
                encRexpr = encryptExpr(expr->rexpr, t2, info);
        }

        funcCall->args = lappend(funcCall->args, encLexpr);
        funcCall->args = lappend(funcCall->args, encRexpr);
<<<<<<< HEAD
        ans = (Node *)funcCall;
    }
    else
    {

=======

        //如果不使用SGX，添加AES列也没有意义, 但是ore_min必须要添加AES
        // #ifdef USE_SGX
        Node *encLAESexpr = encryptExpr(expr->lexpr, CIPHER_AES, info);
        funcCall->args = lappend(funcCall->args, encLAESexpr);
        if (hasSecondCipherParams)
        {
            Node *encRAESexpr = encryptExpr(expr->rexpr, CIPHER_AES, info);
            funcCall->args = lappend(funcCall->args, encRAESexpr);
        }

        // #else
        //     funcCall->args = lappend(funcCall->args, (Node*)makeString("null"));
        //     if(hasSecondCipherParams)
        //         funcCall->args = lappend(funcCall->args, (Node*)makeString("null"));
        // #endif
        ans = (Node *)funcCall;
    }
    else
    {
        info->isALeftOps = true;
        encLexpr = encryptExpr(expr->lexpr, t1, info);
        info->isALeftOps = false;
        info->isARightOps = true;
        encRexpr = encryptExpr(expr->rexpr, t2, info);
        info->isARightOps = false;
        assert(t1 == CIPHER_AES || t1 == CIPHER_NOCRYPT);
>>>>>>> dbdev
        A_Expr *newExpr = makeNode(A_Expr);
        *newExpr = *expr;
        newExpr->lexpr = encLexpr;
        newExpr->rexpr = encRexpr;
        ans = (Node *)newExpr;
    }

    return ans;
}

<<<<<<< HEAD
static CaseExpr *encryptCaseExpr(CaseExpr *ce)
{
    CaseExpr *newCe = (CaseExpr *)copyObject(ce);
    newCe->arg = (Expr *)encryptDispatcher((Node *)ce->arg, CIPHER_DET);
=======
static CaseExpr *encryptCaseExpr(CaseExpr *ce, EncryptInfo *info)
{
    CaseExpr *newCe = (CaseExpr *)copyObject(ce);
    // newCe->arg = (Expr *)encryptDispatcher((Node *)ce->arg, CIPHER_DET);
    newCe->arg = (Expr *)encryptDispatcher((Node *)ce->arg, CIPHER_AES, info);
>>>>>>> dbdev
    ListCell *cell;
    newCe->args = NIL;
    foreach (cell, ce->args)
    {
        Node *n = (Node *)lfirst(cell);
        if (IsA(n, CaseWhen))
        {
            CaseWhen *newCW = makeNode(CaseWhen);
<<<<<<< HEAD
            newCW->expr = (Expr *)encryptDispatcher((Node *)((CaseWhen *)n)->expr, DEFALUT_ENCRYPT_CIPHER);
            newCW->result = (Expr *)encryptDispatcher((Node *)((CaseWhen *)n)->result, DEFALUT_ENCRYPT_CIPHER);
=======
            newCW->expr = (Expr *)encryptDispatcher((Node *)((CaseWhen *)n)->expr, DEFALUT_ENCRYPT_CIPHER, info);
            newCW->result = (Expr *)encryptDispatcher((Node *)((CaseWhen *)n)->result, DEFALUT_ENCRYPT_CIPHER, info);
>>>>>>> dbdev
            newCe->args = lappend(newCe->args, newCW);
        }
        else
            errmsg("unhandled CaseExpr %d in encryptCaseExpr()\n", nodeTag(n));
    }

<<<<<<< HEAD
    newCe->defresult = (Expr *)encryptDispatcher((Node *)ce->defresult, DEFALUT_ENCRYPT_CIPHER);
    return newCe;
}

static List *encryptList(List *list)
=======
    newCe->defresult = (Expr *)encryptDispatcher((Node *)ce->defresult, DEFALUT_ENCRYPT_CIPHER, info);
    return newCe;
}

static List *encryptList(List *list, EncryptInfo *info)
>>>>>>> dbdev
{
    ListCell *cell;
    List *newList = NIL;
    foreach (cell, list)
    {
<<<<<<< HEAD
        newList = lappend(newList, encryptDispatcher((Node *)lfirst(cell), DEFALUT_ENCRYPT_CIPHER));
=======
        newList = lappend(newList, encryptDispatcher((Node *)lfirst(cell), DEFALUT_ENCRYPT_CIPHER, info));
>>>>>>> dbdev
    }
    return newList;
}

<<<<<<< HEAD
static Node *encryptDispatcher(Node *expr, T_Cipher t)
=======
static Node *encryptDispatcher(Node *expr, T_Cipher t, EncryptInfo *info)
>>>>>>> dbdev
{
    Node *res = NULL;
    if (!expr)
        return res;
    switch (nodeTag(expr))
    {
    case T_ColumnRef:
<<<<<<< HEAD
        res = (Node *)encryptColumnRef((ColumnRef *)expr, t);
        break;
    case T_A_Const:
        res = (Node *)encryptAConst((A_Const *)expr, t);
        break;
    case T_FuncCall:
        res = (Node *)encryptFuncCall((FuncCall *)expr);
        break;
    case T_A_Expr:
        res = (Node *)encryptAExpr((A_Expr *)expr);
        break;
    case T_SubLink:
        res = (Node *)encryptSubLink((SubLink *)expr);
        break;
    case T_CaseExpr:
        res = (Node *)encryptCaseExpr((CaseExpr *)expr);
        break;
    case T_List:
        res = (Node *)encryptList((List *)expr);
=======
        res = (Node *)encryptColumnRef((ColumnRef *)expr, t, info);
        break;
    case T_A_Const:
        res = (Node *)encryptAConst((A_Const *)expr, t, info);
        break;
    case T_FuncCall:
        res = (Node *)encryptFuncCall((FuncCall *)expr, info);
        break;
    case T_A_Expr:
        res = (Node *)encryptAExpr((A_Expr *)expr, info);
        break;
    case T_SubLink:
        res = (Node *)encryptSubLink((SubLink *)expr, info);
        break;
    case T_CaseExpr:
        res = (Node *)encryptCaseExpr((CaseExpr *)expr, info);
        break;
    case T_List:
        res = (Node *)encryptList((List *)expr, info);
>>>>>>> dbdev
        break;
    default:
        errmsg("unrecognized node type: %d in encryptDispatcher()", (int)nodeTag(expr));
        break;
    }
    return res;
}

<<<<<<< HEAD
static List *encryptFunctionArgs(List *args, const char *funcName)
{
    List *newList = NIL;
    ListCell *arg;
    foreach (arg, args)
    {
        Node *n = (Node *)lfirst(arg);
        newList = lappend(newList, encryptDispatcher(n, getCipherTypeByOp(funcName, false)));
=======
bool funcCallNeedAESCol(const char *funcName)
{
    if (strcmp(funcName, "count") == 0)
        return false;
    return true;
}

static List *encryptFunctionArgs(List *args, const char *funcName, EncryptInfo *info)
{
    List *newList = NIL;
    ListCell *arg;
    int cnt = 0;
    foreach (arg, args)
    {
        ++cnt;
        Node *n = (Node *)lfirst(arg);
        T_Cipher t = getCipherTypeByOp(funcName, false);
        if (cnt == 2)
        {
            if (!strcmp(funcName, "pow")) //pow(id,INT)
                t = CIPHER_NOCRYPT;
        }
        newList = lappend(newList, encryptDispatcher(n, t, info));
        if (t != CIPHER_NOCRYPT && funcCallNeedAESCol(funcName))
            newList = lappend(newList, encryptDispatcher(n, CIPHER_AES, info));
>>>>>>> dbdev
    }
    return newList;
}

<<<<<<< HEAD
static List *encryptFunctionName(List *funcNames)
=======
static List *encryptFunctionName(List *funcNames, EncryptInfo *info)
>>>>>>> dbdev
{
    List *newNameList = NIL;
    ListCell *funcNameCell;
    foreach (funcNameCell, funcNames)
    {
        Value *funcName = (Value *)lfirst(funcNameCell);
        Value *UDFfuncName = makeString(getMappedName(T_STRING_UDF_FUNCTION, strVal(funcName), strVal(funcName)));
        // 如果map文件里没有funcname的映射udf，则返回原来的名字
        newNameList = lappend(newNameList, UDFfuncName);
    }
    return newNameList;
}

static Node *dealWithSpecialFunction(FuncCall *func, EncryptInfo *info)
{
    if (!func)
        return nullptr;
    char *funcname = (char *)strVal((Value *)llast(func->funcname));
    Node *ret = nullptr;
    if (!strcmp(funcname, "now"))
    {
        int64_t now = GetCurrentTimestamp();
        // std::string strNow = ltos(now);
        ret = (Node *)makeNode(A_Const);
        ((A_Const *)ret)->val = *makeInteger(now);
    }

    return ret;
}

<<<<<<< HEAD
static FuncCall *encryptFuncCall(FuncCall *func)
{
    Node *tmp = dealWithSpecialFunction(func);
=======
static FuncCall *encryptFuncCall(FuncCall *func, EncryptInfo *info)
{
    Node *tmp = dealWithSpecialFunction(func, info);
>>>>>>> dbdev
    if (tmp)
        return (FuncCall *)tmp;

    FuncCall *newFuncCall = makeNode(FuncCall);
    *newFuncCall = *func;
    newFuncCall->funcname = encryptFunctionName(func->funcname, info);
    newFuncCall->args = encryptFunctionArgs(func->args, strVal(linitial(func->funcname)), info);
    return newFuncCall;
}

<<<<<<< HEAD
static List *encryptTargetList(List *targetlist, bool isInsert) // 如果是来自insert，则target列需要进行
=======
static List *encryptTargetList(List *targetlist, bool isInsert, EncryptInfo *info) // 如果是来自insert，则target列需要进行
>>>>>>> dbdev
{
    List *p_target = NIL;
    ListCell *o_target;

    foreach (o_target, targetlist)
    {
        ResTarget *res = (ResTarget *)lfirst(o_target);
        ResTarget *newRes = makeNode(ResTarget);

        *newRes = *res;
        pfree(newRes->val);

        if (IsA(res->val, FuncCall))
        {
            Node *tmpRes = dealWithSpecialFunction((FuncCall *)res->val, info);
            res->val = tmpRes ? tmpRes : res->val;
        }

        if (isInsert)
        {
            ColumnRef *ref = (ColumnRef *)res->val;
            char *colname = (char *)strVal((Value *)llast(ref->fields)); // 假设来自insert语句的select，那么ref->fields里面存储的都是columnname
            int ncipher = 0;
            T_Cipher ciphers[CIPHER_COUNT];

            getColumnCiphers(colname, ciphers, &ncipher);

            for (int i = 0; i < ncipher; ++i)
            {
                newRes = makeNode(ResTarget);
                newRes->val = encryptDispatcher(res->val, ciphers[i], info);
                p_target = lappend(p_target, newRes);
            }
        }

        else
        {
<<<<<<< HEAD
            newRes->val = encryptDispatcher(res->val, DEFALUT_COLUMN_ENCRYPT_CIPHER);
=======
            newRes->val = encryptDispatcher(res->val, DEFALUT_ENCRYPT_CIPHER, info);
>>>>>>> dbdev
            p_target = lappend(p_target, newRes);
        }

        char *aliasname = res->name;

        if (IsA(res->val, A_Expr) || IsA(res->val, FuncCall))
        {
            T_Cipher t = getEncryptTypeFromNode(res->val);
            //TODO: BUG 如果是insert into stu(id) select id from stu1; 那么就不需要替换
            if (t == CIPHER_ORE)
                t = DEFALUT_ENCRYPT_CIPHER; // ORE 不能解密
            aliasname = aliasname ? addEncryptSubfix(t, aliasname) : addEncryptSubfix(t, "unnamedColumn");
        }
        if (aliasname)
        { // 加密 AS 别称
            newRes->name = setMappedName(T_STRING_RESTMP, aliasname, getRandomBytesInHex(MAP_NAME_BUFFER_SIZE));
        }
    }
    return p_target;
}

<<<<<<< HEAD
static void replaceColumnDefType(TypeName *_typename, const char *type, T_Cipher t)
=======
static void replaceColumnDefType(TypeName *_typename, const char *type, T_Cipher t, EncryptInfo *info)
>>>>>>> dbdev
{
    char *oldType = strVal(llast(_typename->names)); // WARN typename 长度是多少呢？

    if (t == CIPHER_NOCRYPT) // 如果这一列不需要加密，也就不需要替换类型。
        return;

    if (strcmp(oldType, type))
    { //不相等
        List *listType = _typename->names;
        listType = list_delete(listType, llast(listType));
        listType = lappend(listType, makeString((char *)type));
    }
    // A_Const* param = makeNode(A_Const);
    // param->val = *makeInteger(getCipherFormatSize(CIPHER_FORMAT, t));
    _typename->typmods = NULL;
}

<<<<<<< HEAD
static List *encryptColumnDefToList(ColumnDef *columnDef, const char *_typename) // for create clause only
=======
static List *encryptColumnDefToList(ColumnDef *columnDef, const char *_typename, EncryptInfo *info) // for create clause only
>>>>>>> dbdev
{
    // create stmt 中需要将一个列扩展成多个列
    List *ansList = NIL;
    Constraint *uniqueConstraint = NULL;
    bool hasPrimary = hasConstraintAndReturn(columnDef->constraints, CONSTR_PRIMARY, &uniqueConstraint);
    if (hasPrimary)
        columnDef->constraints = removeConstraints(columnDef->constraints, CONSTR_PRIMARY);

    T_Cipher *ciphers = (T_Cipher *)palloc(sizeof(int) * CIPHER_COUNT);
    int actualnCipher = 0;
    getCiphersByTypename(_typename, (int *)ciphers, &actualnCipher); // 根据typename设置ciphpers
    for (int i = 0; i < actualnCipher; i++)
    { // we dont need the last two item of T_Cipher
        T_Cipher tCipher = ciphers[i];
        char *cryptName = nullptr;
        ColumnDef *newDef = (ColumnDef *)copyObject(columnDef);
        char *colname = addEncryptSubfix(tCipher, columnDef->colname);
        size_t size = MAP_NAME_BUFFER_SIZE;

        if (tCipher != CIPHER_NOCRYPT)
            cryptName = getRandomBytesInHex(size);
        else
            cryptName = columnDef->colname;

        replaceColumnDefType(newDef->typeName, DATABASE_CIPHER_TYPE, tCipher); // 把原始类型替换为密文类型， 需要对应密文的长度

        if (hasPrimary && (tCipher == DEFALUT_PRIMARY_KEY_CIPHER || tCipher == CIPHER_NOCRYPT))
            // 如果原来的列有主键，那么主键需要加在DET或者NoCrypt的列上。
            newDef->constraints = lappend(newDef->constraints, uniqueConstraint);
        cryptName = setMappedName(T_STRING_COLUMN, colname, cryptName);

        newDef->colname = cryptName;
        ansList = lappend(ansList, newDef);
    }

    return ansList;
}

<<<<<<< HEAD
static List *encrypttableElts(List *tableElts) // for create clause only
=======
static List *encrypttableElts(List *tableElts, EncryptInfo *info) // for create clause only
>>>>>>> dbdev
{
    List *newCdefList = NIL;
    ListCell *columnDefCell;
    foreach (columnDefCell, tableElts)
    {
        Node *n = (Node *)lfirst(columnDefCell);

        if (IsA(n, ColumnDef))
        {
            ColumnDef *columnDef = (ColumnDef *)n;
            TypeName *_typeName = columnDef->typeName;

            char *__typename = strVal(llast(_typeName->names));
            addColumnAndType(columnDef->colname, __typename); // add column to mapper
            newCdefList = list_concat(newCdefList, encryptColumnDefToList(columnDef, __typename));
        }
        else if (IsA(n, Constraint))
        {
<<<<<<< HEAD
            newCdefList = lappend(newCdefList, (Node *)encryptExplicitConstraint((Constraint *)n));
=======
            newCdefList = lappend(newCdefList, (Node *)encryptExplicitConstraint((Constraint *)n, info));
>>>>>>> dbdev
        }
    }
    return newCdefList;
}

<<<<<<< HEAD
static Constraint *encryptExplicitConstraint(Constraint *con) // 将显示声明的constraint扩展多次
=======
static Constraint *encryptExplicitConstraint(Constraint *con, EncryptInfo *info) // 将显示声明的constraint扩展多次
>>>>>>> dbdev
{
    // int a = 1;
    List *ansKeys = NIL;
    Constraint *newCon = (Constraint *)copyObject(con);
    ListCell *cell = NULL, *cell2 = NULL;
    List *colList = NIL;
    List *pkList = NIL;
    List *ansKeys2 = NIL;

    if (con->contype == CONSTR_PRIMARY || con->contype == CONSTR_UNIQUE)
        colList = con->keys;

    else if (con->contype == CONSTR_FOREIGN)
    {
        colList = con->fk_attrs;
        pkList = con->pk_attrs;
        cell2 = list_head(pkList);

        if (con->pktable)
<<<<<<< HEAD
            newCon->pktable = encryptRangeVar(con->pktable, NULL);
=======
            newCon->pktable = encryptRangeVar(con->pktable, NULL, info);
>>>>>>> dbdev
    }

    foreach (cell, colList)
    {
        const char *colname = strVal((Value *)lfirst(cell));
        int ncipher = 0;
        T_Cipher ciphers[CIPHER_COUNT];
        getColumnCiphers(colname, ciphers, &ncipher);
        std::set<T_Cipher> setCiphers(ciphers, ciphers + ncipher);

        std::vector<T_Cipher> vectCiphers;

        if (con->contype == CONSTR_PRIMARY || con->contype == CONSTR_UNIQUE || con->contype == CONSTR_FOREIGN)
        { // 如果是 primary key，只添加到一个加密列上。
            // TODO: 外键是否也加到同一个列上?
            if (setCiphers.count(DEFALUT_PRIMARY_KEY_CIPHER))
                vectCiphers.push_back(DEFALUT_PRIMARY_KEY_CIPHER);
            else if (setCiphers.count(CIPHER_NOCRYPT))
                vectCiphers.push_back(CIPHER_NOCRYPT);
            else
            {
                throw("we have no column to add constraint key \n");
            }
        }
        else
        { // 不是上述所有情况，默认添加到所有加密列上
            vectCiphers.assign(ciphers, ciphers + ncipher);
        }

        for (auto &x : vectCiphers)
        {
            char *ciphercolname = getMappedName(T_STRING_COLUMN, addEncryptSubfix(x, colname), NULL);
            ansKeys = lappend(ansKeys, makeString(ciphercolname));

            if (cell2)
            {
                char *fkcolname = strVal((Value *)lfirst(cell2));
                ansKeys2 = lappend(ansKeys2, makeString(getMappedName(T_STRING_COLUMN, addEncryptSubfix(x, fkcolname), NULL)));
            }
        }

        if (cell2 && cell2->next)
            cell2 = lnext(cell2);
    }

    if (con->contype == CONSTR_PRIMARY || con->contype == CONSTR_UNIQUE)
        newCon->keys = ansKeys;

    else if (con->contype == CONSTR_FOREIGN)
    {
        newCon->fk_attrs = ansKeys;
        newCon->pk_attrs = ansKeys2;
    }

    return newCon;
}

<<<<<<< HEAD
static RangeSubselect *encryptRangeSubselect(RangeSubselect *rss)
=======
static RangeSubselect *encryptRangeSubselect(RangeSubselect *rss, EncryptInfo *info)
>>>>>>> dbdev
{
    RangeSubselect *newRss = makeNode(RangeSubselect);
    newRss->alias = encryptAlias(rss->alias);
    newRss->subquery = encryptStmt(rss->subquery, info);
    return newRss;
}

<<<<<<< HEAD
static Alias *encryptAlias(Alias *alias)
=======
static Alias *encryptAlias(Alias *alias, EncryptInfo *info)
>>>>>>> dbdev
{
    if (!alias)
        return NULL;
    Alias *newAlias = makeNode(Alias);
    newAlias->aliasname = setMappedName(T_STRING_TBLTMP, alias->aliasname, getRandomBytesInHex(MAP_NAME_BUFFER_SIZE));
    newAlias->colnames = (List *)copyObject(alias->colnames); // TEST: What the fuck is this?
    return newAlias;
}

<<<<<<< HEAD
static char *encryptRVField(PlainDataType p_type, bool *isCreate, char *name)
=======
static char *encryptRVField(PlainDataType p_type, bool *isCreate, char *name, EncryptInfo *info)
>>>>>>> dbdev
{
    char *ans = NULL;
    if (!isCreate)
        ans = getMappedName(p_type, name, NULL);
    else
    {
        size_t size = MAP_NAME_BUFFER_SIZE;
        ans = getMappedName(p_type, name, "");
        if (!*ans)
        { // 如果没有记录
            char *cryptName;
            cryptName = getRandomBytesInHex(size);
            cryptName = setMappedName(p_type, name, cryptName);
            ans = cryptName;
        }
        else
            *isCreate = false;
    }
    return ans;
}

<<<<<<< HEAD
static RangeVar *encryptRangeVar(RangeVar *rv, bool *isCreate) // isCreate字段为nullptr代表false
=======
static RangeVar *encryptRangeVar(RangeVar *rv, bool *isCreate, EncryptInfo *info) // isCreate字段为nullptr代表false
>>>>>>> dbdev
{
    RangeVar *newrv = makeNode(RangeVar);
    if (rv->catalogname)
    {
        newrv->catalogname = encryptRVField(T_STRING_CATALOG, isCreate, rv->catalogname);
    }
    if (rv->schemaname)
    {
        if (strcmp(rv->schemaname, "pg_catalog") == 0)
            return nullptr;
        newrv->schemaname = encryptRVField(T_STRING_SCHEMA, isCreate, rv->schemaname);
        setCurrentNodeOnce("", rv->schemaname);
    }
    if (rv->relname)
    {
        newrv->relname = encryptRVField(T_STRING_TABLE, isCreate, rv->relname);
        if (newrv->relname != NOEXIST)
            setFromTable(rv->relname);
        if (rv->alias)
        {
            newrv->alias = encryptAlias(rv->alias);
            setTableAlias(rv->relname, rv->alias->aliasname);
        }
    }

    newrv->relpersistence = rv->relpersistence;
    newrv->inhOpt = rv->inhOpt; // TEST: Don't know whats this is?
    return newrv;
}

<<<<<<< HEAD
static JoinExpr *encryptJoinExpr(JoinExpr *joinExpr)
=======
static JoinExpr *encryptJoinExpr(JoinExpr *joinExpr, EncryptInfo *info)
>>>>>>> dbdev
{
    JoinExpr *newJoinExpr = makeNode(JoinExpr);
    newJoinExpr->jointype = joinExpr->jointype;
    newJoinExpr->isNatural = joinExpr->isNatural;
<<<<<<< HEAD
    newJoinExpr->larg = (Node *)encryptRangeVar((RangeVar *)joinExpr->larg, NULL);
    newJoinExpr->rarg = (Node *)encryptRangeVar((RangeVar *)joinExpr->rarg, NULL);
    newJoinExpr->usingClause = (List *)copyObject(joinExpr->usingClause); // TEST: not process yet.
    newJoinExpr->quals = (Node *)encryptAExpr((A_Expr *)joinExpr->quals); // ON keyword
=======
    newJoinExpr->larg = (Node *)encryptRangeVar((RangeVar *)joinExpr->larg, NULL, info);
    newJoinExpr->rarg = (Node *)encryptRangeVar((RangeVar *)joinExpr->rarg, NULL, info);
    newJoinExpr->usingClause = (List *)copyObject(joinExpr->usingClause); // TEST: not process yet.
    newJoinExpr->quals = (Node *)encryptAExpr((A_Expr *)joinExpr->quals,info); // ON keyword
>>>>>>> dbdev
    newJoinExpr->alias = encryptAlias(joinExpr->alias);
    newJoinExpr->rtindex = joinExpr->rtindex;
    return newJoinExpr;
}

<<<<<<< HEAD
static List *encryptSortClause(List *SortClause)
=======
static List *encryptSortClause(List *SortClause, EncryptInfo *info)
>>>>>>> dbdev
{
    List *newSortList = NIL;
    ListCell *sort;

    foreach (sort, SortClause)
    {
        SortBy *n = (SortBy *)lfirst(sort);
        SortBy *newSort = (SortBy *)copyObject(n);

        if (IsA(n->node, A_Const))
        {
            newSort->node = (Node *)copyObject(n->node);
        }
        else
<<<<<<< HEAD
            newSort->node = encryptDispatcher(n->node, CIPHER_ORE);
=======
#if defined(SGX_ORE) || defined(SGX_MHE)
            newSort->node = encryptDispatcher(n->node, CIPHER_AES, info);
#else
            newSort->node = encryptDispatcher(n->node, CIPHER_ORE, info);
#endif
>>>>>>> dbdev
        newSortList = lappend(newSortList, newSort); // TEST: order by 应该是一个判断大小的操作
    }
    return newSortList;
}

<<<<<<< HEAD
static List *encryptGroupClause(List *groupClause)
=======
static List *encryptGroupClause(List *groupClause, EncryptInfo *info)
>>>>>>> dbdev
{
    List *newGroupList = NIL;
    ListCell *group;

    foreach (group, groupClause)
    {
        Node *n = (Node *)lfirst(group);
        if (IsA(n, ColumnRef))
        {
            newGroupList = lappend(
<<<<<<< HEAD
                newGroupList, encryptColumnRef((ColumnRef *)n, CIPHER_DET)); // TEST: group by 应该是一个判断等于的操作
=======
                // newGroupList, encryptColumnRef((ColumnRef *)n, CIPHER_DET)); // TEST: group by 应该是一个判断等于的操作
                newGroupList, encryptColumnRef((ColumnRef *)n, CIPHER_AES,info));
>>>>>>> dbdev
        }
        else
        {
            errmsg("unrecognized node type: %d in encryptGroupClause()", (int)nodeTag(n));
        }
    }
    return newGroupList;
}

<<<<<<< HEAD
static List *encryptFromClause(List *fromClause)
=======
static List *encryptFromClause(List *fromClause, EncryptInfo *info)
>>>>>>> dbdev
{
    List *newfrmList = NIL;
    ListCell *from;
    int i = 0;
    foreach (from, fromClause)
    {
        Node *n = (Node *)lfirst(from);
        if (!n) // 解析完毕还有List里面的元素是NULL的情况。
            break;
        i++;
        if (IsA(n, RangeVar))
        {
<<<<<<< HEAD
            auto node = encryptRangeVar((RangeVar *)n, NULL);
=======
            auto node = encryptRangeVar((RangeVar *)n, NULL, info);
>>>>>>> dbdev
            if (node == nullptr)
                return (List *)node;
            newfrmList = lappend(newfrmList, node);
        }
        else if (IsA(n, RangeSubselect))
        {
            newfrmList = lappend(newfrmList, encryptRangeSubselect((RangeSubselect *)n));
        }

        // else if (IsA(n, RangeFunction))
        // { // TEST: Not Known what is this;
        //     // newfrmList = lappend(newfrmList, encryptFuncCall((RangeFunction *)n));
        // }
        else if (IsA(n, JoinExpr))
        {
            newfrmList = lappend(newfrmList, encryptJoinExpr((JoinExpr *)n));
        }

        else
        {
            errmsg("unrecognized node type: %d in encryptFromClause()", (int)nodeTag(n));
        }
    }
    return newfrmList;
}

<<<<<<< HEAD
static List *encryptExtendTargetList(List *targetList) // 用于处理updatestmt insertStmt 中的columnref
=======
static List *encryptExtendTargetList(List *targetList, EncryptInfo *info) // 用于处理updatestmt insertStmt 中的columnref
>>>>>>> dbdev
{
    ListCell *cell;
    List *ansList = NIL;
    foreach (cell, targetList)
    {
        ResTarget *o_target = (ResTarget *)lfirst(cell);

        T_Cipher ciphers[CIPHER_COUNT];

        if (o_target->val && IsA(o_target->val, FuncCall))
        {
            Node *tmpRes = dealWithSpecialFunction((FuncCall *)o_target->val, info);
            o_target->val = tmpRes ? tmpRes : o_target->val;
        }

        int nCiphers = 0;
        getColumnCiphers(o_target->name, ciphers, &nCiphers);

        for (int i = 0; i < nCiphers; ++i)
        {
<<<<<<< HEAD
=======
            // if(ciphers[i] == CIPHER_AES) {
            //     // done: for debug, remove this in release mode
            //     continue;
            // }
>>>>>>> dbdev
            ResTarget *newRS = makeNode(ResTarget);
            *newRS = *o_target;
            if (o_target->name)
            {
                char *colname = o_target->name;
                colname = addEncryptSubfix(ciphers[i], colname);
                newRS->name = getMappedName(T_STRING_COLUMN, colname, colname);
                if (o_target->val)                                                   // Is A Expr
                    newRS->val = encryptDispatcher(o_target->val, ciphers[i], info); //TODO:  wantt

                ansList = lappend(ansList, newRS);
            }
            else
            {
                errmsg("updateStmt has no name in encryptExtendTargetList()\n");
            }
        }
    }
    return ansList;
}

<<<<<<< HEAD
static List *encryptValuesLists(List *valuesLists, List *cols)
=======
static List *encryptValuesLists(List *valuesLists, List *cols, EncryptInfo *info)
>>>>>>> dbdev
{
    // Not CHange
    ListCell *outCell;
    List *ansList = NIL;

    ColumnInfo **colInfos = NULL;
    int nInfo = 0;

    if (!cols) // 没有指定列名，默认用所有的列
        getFirstTableAllColumnsInfoAlloc(&colInfos, &nInfo);
    else
    {
        getSpecifyColumnsInfoAlloc(cols, &colInfos);
        nInfo = cols->length;
    }

    if (!colInfos)
        return NIL;

    foreach (outCell, valuesLists)
    {
        List *valueList = (List *)lfirst(outCell);
        List *newValueList = NIL;
        ListCell *innerCell;
        int p_colval = 0;
        foreach (innerCell, valueList)
        {
<<<<<<< HEAD
            ColumnInfo *info = colInfos[p_colval];
            Node *n = (Node *)lfirst(innerCell);

            if (IsA(n, FuncCall))
            {
                Node *tmpRes = dealWithSpecialFunction((FuncCall *)n);
=======
            ColumnInfo *colInfo = colInfos[p_colval];
            Node *n = (Node *)lfirst(innerCell);
            if (string(colInfo->type).find("float") != string::npos)
            {
                info->isFloatCol = true;
            }
            else
            {
                info->isFloatCol = false;
            }
            if (IsA(n, FuncCall))
            {
                Node *tmpRes = dealWithSpecialFunction((FuncCall *)n, info);
>>>>>>> dbdev
                n = tmpRes ? tmpRes : n;
            }

            for (size_t i = 0; i < colInfo->len; i++)
            {
<<<<<<< HEAD
                Node *ans = encryptDispatcher(n, info->ciphers[i]);
=======

                Node *ans = encryptDispatcher(n, colInfo->ciphers[i], info);
>>>>>>> dbdev
                newValueList = lappend(newValueList, ans);
            }

            p_colval++;
        }
        ansList = lappend(ansList, newValueList);
    }
    return ansList;
}

// static inline T_Cipher getCipherByString(const std::string& info) {
//     if (!info.compare("sum") || !info.compare("sub") || !info.compare("+") || !info.compare("-"))
//         return CIPHER_SAHE;

//     if (!info.compare("div") ||  !info.compare("*") || !info.compare("/"))
//         return CIPHER_SMHE;

//     return DEFALUT_ENCRYPT_CIPHER;
// }

<<<<<<< HEAD
static T_Cipher getEncryptTypeFromNode(Node *n)
=======
static T_Cipher getEncryptTypeFromNode(Node *n, EncryptInfo *info)
>>>>>>> dbdev
{
    char *name = nullptr;
    switch (n->type)
    {

    case T_A_Expr:
        name = strVal(((Value *)llast(((A_Expr *)n)->name)));
        break;

    case T_FuncCall:
        name = strVal(((Value *)llast(((FuncCall *)n)->funcname)));
        break;

    default:
        break;
    }
    return getCipherTypeByOp(name, true);
}

<<<<<<< HEAD
static InsertStmt *encryptInsertStmt(InsertStmt *parseTree)
=======
static InsertStmt *encryptInsertStmt(InsertStmt *parseTree, EncryptInfo *info)
>>>>>>> dbdev
{
    InsertStmt *newTree = makeNode(InsertStmt);
    *newTree = *parseTree;
    newTree->relation = encryptRangeVar(parseTree->relation, NULL, info);
    if (parseTree->cols)
<<<<<<< HEAD
        newTree->cols = encryptExtendTargetList(parseTree->cols);
    if (parseTree->selectStmt)
        newTree->selectStmt = (Node *)encryptSelectStmt((SelectStmt *)parseTree->selectStmt, parseTree->cols);
=======
        newTree->cols = encryptExtendTargetList(parseTree->cols, info);
    if (parseTree->selectStmt)
        newTree->selectStmt = (Node *)encryptSelectStmt((SelectStmt *)parseTree->selectStmt, parseTree->cols, info);
>>>>>>> dbdev

    return newTree;
}

<<<<<<< HEAD
static DeleteStmt *encryptDeleteStmt(DeleteStmt *parseTree)
=======
static DeleteStmt *encryptDeleteStmt(DeleteStmt *parseTree, EncryptInfo *info)
>>>>>>> dbdev
{
    DeleteStmt *newTree = makeNode(DeleteStmt);
    *newTree = *parseTree;
    newTree->relation = encryptRangeVar(parseTree->relation, NULL, info);
    if (parseTree->whereClause)
        newTree->whereClause = encryptDispatcher(parseTree->whereClause, DEFALUT_ENCRYPT_CIPHER, info);
    return newTree;
}

<<<<<<< HEAD
static UpdateStmt *encryptUpdateStmt(UpdateStmt *parseTree)
=======
static UpdateStmt *encryptUpdateStmt(UpdateStmt *parseTree, EncryptInfo *info)
>>>>>>> dbdev
{
    UpdateStmt *newUpdateTree = makeNode(UpdateStmt);
    *newUpdateTree = *parseTree;
    newUpdateTree->relation = encryptRangeVar(parseTree->relation, NULL, info);
    if (parseTree->fromClause)
        newUpdateTree->fromClause = encryptFromClause(parseTree->fromClause, info);
    if (parseTree->whereClause)
        newUpdateTree->whereClause = encryptDispatcher(parseTree->whereClause, DEFALUT_ENCRYPT_CIPHER, info);
    newUpdateTree->targetList = encryptExtendTargetList(parseTree->targetList, info);

    return newUpdateTree;
}

<<<<<<< HEAD
static SelectStmt *encryptSelectStmt(SelectStmt *parseTree, List *cols)
{
=======
static SelectStmt *encryptSelectStmt(SelectStmt *parseTree, List *cols, EncryptInfo *info)
{
    info->father = (void*)parseTree;
>>>>>>> dbdev
    SelectStmt *newParseTree = makeNode(SelectStmt);
    *newParseTree = *parseTree;

    if (parseTree->fromClause)
    {
<<<<<<< HEAD
        newParseTree->fromClause = (List *)encryptFromClause(parseTree->fromClause);
=======
        newParseTree->fromClause = (List *)encryptFromClause(parseTree->fromClause, info);
>>>>>>> dbdev
        if (newParseTree->fromClause == nullptr)
            return nullptr;
    }

    if (parseTree->targetList)
    {
<<<<<<< HEAD
        newParseTree->targetList = (List *)encryptTargetList(parseTree->targetList, (bool)cols);
=======
        newParseTree->targetList = (List *)encryptTargetList(parseTree->targetList, (bool)cols, info);
>>>>>>> dbdev
    }

    if (parseTree->whereClause)
    {
        Node *n = parseTree->whereClause;
<<<<<<< HEAD
        newParseTree->whereClause = encryptDispatcher(n, DEFALUT_ENCRYPT_CIPHER);
=======
        newParseTree->whereClause = encryptDispatcher(n, DEFALUT_ENCRYPT_CIPHER, info);
>>>>>>> dbdev
    }

    if (parseTree->groupClause)
    {
<<<<<<< HEAD
        newParseTree->groupClause = encryptGroupClause(parseTree->groupClause);
=======
        newParseTree->groupClause = encryptGroupClause(parseTree->groupClause, info);
>>>>>>> dbdev
    }

    if (parseTree->havingClause)
    {
        // TEST: having 常见的应该是 表达式 > Const， 处理不了。
    }

    if (parseTree->sortClause)
    {
<<<<<<< HEAD
        newParseTree->sortClause = encryptSortClause(parseTree->sortClause);
=======
        newParseTree->sortClause = encryptSortClause(parseTree->sortClause, info);
>>>>>>> dbdev
    }

    if (parseTree->valuesLists)
    {
<<<<<<< HEAD
        newParseTree->valuesLists = encryptValuesLists(parseTree->valuesLists, cols);
=======
        newParseTree->valuesLists = encryptValuesLists(parseTree->valuesLists, cols, info);
>>>>>>> dbdev
    }

    if (parseTree->lockingClause)
        newParseTree->lockingClause = (List *)copyObject(parseTree->lockingClause);

    return newParseTree;
}

<<<<<<< HEAD
static CreateStmt *encryptCreateStmt(CreateStmt *parseTree)
=======
static CreateStmt *encryptCreateStmt(CreateStmt *parseTree, EncryptInfo *info)
>>>>>>> dbdev
{
    CreateStmt *newParseTree = makeNode(CreateStmt);
    *newParseTree = *parseTree;
    bool isCreate = true;
    newParseTree->relation = encryptRangeVar(parseTree->relation, &isCreate, info);
    if (isCreate)
        newParseTree->tableElts = encrypttableElts(parseTree->tableElts, info);
    else
        newParseTree->tableElts = NULL;
    return newParseTree;
}

<<<<<<< HEAD
static List *encryptAlterTableCmds(List *cmdList)
=======
static List *encryptAlterTableCmds(List *cmdList, EncryptInfo *info)
>>>>>>> dbdev
{
    List *ansList = NIL;
    ListCell *cell;
    foreach (cell, cmdList)
    {
        Node *n = (Node *)lfirst(cell);

        Node *ans = nullptr;
        if (IsA(n, AlterTableCmd))
        {
            AlterTableCmd *atcmd = (AlterTableCmd *)n;
            AlterTableCmd *newCmd = makeNode(AlterTableCmd);
            *newCmd = *atcmd; // shallow copy

            if (IsA(atcmd, AlterTableCmd))
            {
<<<<<<< HEAD
                newCmd->def = (Node *)encryptExplicitConstraint((Constraint *)atcmd->def);
=======
                newCmd->def = (Node *)encryptExplicitConstraint((Constraint *)atcmd->def, info);
>>>>>>> dbdev
            }

            ans = (Node *)newCmd;
        }

        ansList = lappend(ansList, ans);
    }
    return ansList;
}

<<<<<<< HEAD
static AlterTableStmt *encryptAlterTableStmt(AlterTableStmt *parseTree)
=======
static AlterTableStmt *encryptAlterTableStmt(AlterTableStmt *parseTree, EncryptInfo *info)
>>>>>>> dbdev
{
    AlterTableStmt *newParseTree = (AlterTableStmt *)makeNode(AlterTableStmt);

    if (parseTree->relation)
<<<<<<< HEAD
        newParseTree->relation = encryptRangeVar(parseTree->relation, NULL);

    if (parseTree->cmds)
        newParseTree->cmds = encryptAlterTableCmds(parseTree->cmds);
=======
        newParseTree->relation = encryptRangeVar(parseTree->relation, NULL, info);

    if (parseTree->cmds)
        newParseTree->cmds = encryptAlterTableCmds(parseTree->cmds, info);
>>>>>>> dbdev

    return newParseTree;
}

<<<<<<< HEAD
static IndexStmt *encryptIndexStmt(IndexStmt *parseTree)
{

=======
static IndexStmt *encryptIndexStmt(IndexStmt *parseTree, EncryptInfo *info)
{
>>>>>>> dbdev
    IndexStmt *newParseTree = (IndexStmt *)copyObject(parseTree);
    newParseTree->indexParams = NIL;
    ListCell *cell = nullptr;

    newParseTree->relation = encryptRangeVar(parseTree->relation, nullptr, info);

    foreach (cell, parseTree->indexParams)
    {
        IndexElem *oldelem = (IndexElem *)lfirst(cell);
        IndexElem *elem = (IndexElem *)copyObject(oldelem);
        int ncipher = 0;
        T_Cipher ciphers[CIPHER_COUNT];
        getColumnCiphers(elem->name, ciphers, &ncipher);

        std::set<T_Cipher> setCiphers(ciphers, ciphers + ncipher);

<<<<<<< HEAD
        if (setCiphers.count(CIPHER_DET)) // 如果有DET, index创建在det上
            elem->name = getMappedName(T_STRING_COLUMN, addEncryptSubfix(CIPHER_DET, elem->name), NULL);
=======
        // if (setCiphers.count(CIPHER_DET)) // 如果有DET, index创建在det上
        //     elem->name = getMappedName(T_STRING_COLUMN, addEncryptSubfix(CIPHER_DET, elem->name), NULL);
        // 创建index索引的顺序
        if (setCiphers.count(CIPHER_AES)) // 如果有DET, index创建在det上
            elem->name = getMappedName(T_STRING_COLUMN, addEncryptSubfix(CIPHER_AES, elem->name), NULL);

>>>>>>> dbdev
        else if (setCiphers.count(CIPHER_NOCRYPT)) // 序列serial类型
            elem->name = getMappedName(T_STRING_COLUMN, addEncryptSubfix(CIPHER_NOCRYPT, elem->name), NULL);
        else if (setCiphers.count(CIPHER_ORE))
            elem->name = getMappedName(T_STRING_COLUMN, addEncryptSubfix(CIPHER_ORE, elem->name), NULL);
        else
            throw "can not find any cipher type to create index on";

        newParseTree->indexParams = lappend(newParseTree->indexParams, elem);
    }
    return newParseTree;
}

<<<<<<< HEAD
static DropStmt *encryptDropStmt(DropStmt *parseTree)
=======
static DropStmt *encryptDropStmt(DropStmt *parseTree, EncryptInfo *info)
>>>>>>> dbdev
{
    DropStmt *newParseTree = makeNode(DropStmt);
    *newParseTree = *parseTree;
    ListCell *ocell;
    List *newList = NIL;
    foreach (ocell, parseTree->objects)
    {
        List *l = NIL;
        Value *v = (Value *)linitial((const List *)lfirst(ocell)); // WARN 只取第一个，不清楚是否还有其他
        Value *newValue;
        const char *newV = NULL;
        if (parseTree->removeType == OBJECT_TABLE)
        {
            newV = getMappedName(T_STRING_TABLE, strVal(v), NOEXIST);
            deleteNode(strVal(v), "");
        }

        else if (parseTree->removeType == OBJECT_SCHEMA)
        {
            newV = getMappedName(T_STRING_SCHEMA, strVal(v), NOEXIST);
            deleteNode("", strVal(v));
        }
        else
            newV = strVal(v);

        newValue = makeString((char *)newV);
        l = lappend(l, newValue);
        newList = lappend(newList, l);
    }
    newParseTree->objects = newList;

    return newParseTree;
}

<<<<<<< HEAD
Node *encryptStmt(Node *parseTree)
{
    Node *newTree = NULL;
    switch (nodeTag(parseTree))
    {
    case T_InsertStmt:
        newTree = (Node *)encryptInsertStmt((InsertStmt *)parseTree);
        break;
    case T_DeleteStmt:
        newTree = (Node *)encryptDeleteStmt((DeleteStmt *)parseTree);
        break;
    case T_UpdateStmt:
        newTree = (Node *)encryptUpdateStmt((UpdateStmt *)parseTree);
        break;
    case T_SelectStmt:
        newTree = (Node *)encryptSelectStmt((SelectStmt *)parseTree, NULL);
        break;
    case T_CreateStmt:
        newTree = (Node *)encryptCreateStmt((CreateStmt *)parseTree);
        break;
    case T_IndexStmt:
        newTree = (Node *)encryptIndexStmt((IndexStmt *)parseTree);
        break;
    case T_DropStmt:
        newTree = (Node *)encryptDropStmt((DropStmt *)parseTree);
        break;
    case T_AlterTableStmt:
        newTree = (Node *)encryptAlterTableStmt((AlterTableStmt *)parseTree);
=======
static ExplainStmt *encryptExplainStmt(ExplainStmt *parseTree, EncryptInfo *info)
{
    ExplainStmt *newParseTree = (ExplainStmt *)copyObject(parseTree);
    *newParseTree = *parseTree;

    newParseTree->query = encryptStmt(parseTree->query, info);
    return newParseTree;
}

Node *encryptStmt(Node *parseTree, EncryptInfo *info)
{
    Node *newTree = NULL;
    info->father = (void*)parseTree;
    switch (nodeTag(parseTree))
    {
    case T_InsertStmt:
        newTree = (Node *)encryptInsertStmt((InsertStmt *)parseTree, info);
        break;
    case T_DeleteStmt:
        newTree = (Node *)encryptDeleteStmt((DeleteStmt *)parseTree, info);
        mapperSave();
        break;
    case T_UpdateStmt:
        newTree = (Node *)encryptUpdateStmt((UpdateStmt *)parseTree, info);
        break;
    case T_SelectStmt:
        newTree = (Node *)encryptSelectStmt((SelectStmt *)parseTree, NULL, info);
        break;
    case T_CreateStmt:
        newTree = (Node *)encryptCreateStmt((CreateStmt *)parseTree, info);
        mapperSave();
        break;
    case T_IndexStmt:
        newTree = (Node *)encryptIndexStmt((IndexStmt *)parseTree, info);
        mapperSave();
        break;
    case T_DropStmt:
        newTree = (Node *)encryptDropStmt((DropStmt *)parseTree, info);
        break;
    case T_AlterTableStmt:
        newTree = (Node *)encryptAlterTableStmt((AlterTableStmt *)parseTree, info);
        
        break;
    case T_ExplainStmt:
        newTree = (Node *)encryptExplainStmt((ExplainStmt *)parseTree, info);
>>>>>>> dbdev
        break;
    default:
        errdebug("unknown parseTree type: %d in encryptStmt()\n", nodeTag(parseTree));
        break;
    }

    return newTree;
}
