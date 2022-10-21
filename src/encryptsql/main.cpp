#include "encryptsql.h"
#include <iostream>

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
#include "encryptsql/node2str.h"

#include "utils/memutils.h"

#ifdef __cplusplus
}
#endif
#include <unistd.h>

<<<<<<< HEAD
#include "encryptsql/utils.h"
#include "encryptsql/fieldmap.h"
=======
#include "utils.h"
#include "encryptsql/fieldmap.h"

#include "Singleton.h"
#include "LRUCache.hpp"
#include "KeyManager.h"
#include "SymAHE.h"
#include "TimeCounter.h"
>>>>>>> dbdev

constexpr int MEMORY_DELETE_LIMIT = 20000;

size_t sql_processed_counter = 0;
<<<<<<< HEAD
=======
#ifdef USE_LRU
// extern LRUCache<long, long> HECache;           // for RandNum(id)  
extern LRUCache<long, std::string> AESCache;   // for AES decrypt
#endif
>>>>>>> dbdev

#ifndef MAPPERPATH
#define MAPPERPATH "/etc/encryptsql/map.json"
#endif

extern "C" List *raw_parser(const char *str);

<<<<<<< HEAD
extern Node *encryptStmt(Node *parseTree);
=======
extern Node *encryptStmt(Node *parseTree, EncryptInfo* info);
>>>>>>> dbdev

extern const char *parseTree2str(Node *parseTree);

extern "C" const char *encryptOneSql(const char *sql) __attribute__((visibility("default")));

extern "C" void disableAsNeeded() __attribute__((visibility("default")));

MemoryContext EncryptContext = NULL;

static bool Init = false;
bool Encrypt = true;
bool SkipDecryptRes = !Encrypt;

void disableAsNeeded()
{
    if (!Init)
        std::cout << "Encryption Library Linked\n";
    return;
}

// #ifdef USE_LRU

// void fillLru()
// {
//     SymAHE symPhe;
//     string key = getAESKey();
//     auto lruCache = Singleton<LRUCache<long, long>>::instance();
//     lruCache->setName("HE");
//     for (int i = 1; i <= lruCache->capacity() / 2; ++i)
//     {
//         long long v = symPhe.RandNum(i);
//     }
//     lruCache->requestTimes = 0;
//     lruCache->hitTimes = 0;
// }
// #endif

bool needDecryptResult(Node *node)
{

    if (node->type == T_ExplainStmt)
    {
        return false;
    }
    return true;
}

bool needSkipEncryption(const char *sql, Node *node = nullptr)
{
    if (sql)
    {
        if (isFilteredSql(sql))
        {
            return true;
        }
    }
    if (node)
    {
        if (node->type == T_VariableShowStmt)
        {
            return true;
        }
    }

    return false;
}

const char *
encryptOneSql(const char *sql)
{
    TimeCounter *counter = Singleton<TimeCounter>::instance();
    Timer globalTimer;
#ifndef USE_ENCRYPT
    Encrypt = false;
    return sql;
#endif
    // std::cout << "main start\n";
    SkipDecryptRes = false;
    if (checkCmd(sql, &Encrypt))
    {
        SkipDecryptRes = true;
        return Encrypt ? "select 1 as enc_status;" : "select 0 as enc_status;"; // 这是设置transform的语句
    }

    if (!Encrypt)
        return sql;

    Node *parseTree = NULL;
    Node *encParseTree = NULL;
    const char *res = NULL;


    if (!Init)
    {
        MemoryContextInit();
        MessageContext = AllocSetContextCreate(TopMemoryContext,
                                               "MessageContext",
                                               ALLOCSET_DEFAULT_MINSIZE,
                                               ALLOCSET_DEFAULT_INITSIZE,
                                               ALLOCSET_DEFAULT_MAXSIZE);
        EncryptContext = AllocSetContextCreate(TopMemoryContext,
                                               "EncryptContext",
                                               ALLOCSET_DEFAULT_MINSIZE,
                                               ALLOCSET_DEFAULT_INITSIZE,
                                               ALLOCSET_DEFAULT_MAXSIZE);

        mapperInit(MAPPERPATH);

#ifdef USE_LRU

    // HECache.setName("Client HE");           // for RandNum(id)  
    AESCache.setName("Client AES"); 
#endif
        Init = true;
    }
    sql_processed_counter++;

<<<<<<< HEAD
    sql_processed_counter++;

=======
>>>>>>> dbdev
    if (sql_processed_counter < MEMORY_DELETE_LIMIT)
    {
        MemoryContextResetChildren(TopMemoryContext);
        // MemoryContextReset(MessageContext);
    }
    else
    {
        sql_processed_counter = 0;
        MemoryContextDeleteChildren(TopMemoryContext);
        // MemoryContextDelete(MessageContext);
    }

<<<<<<< HEAD
    if (isFilteredSql(sql))
=======
    if (needSkipEncryption(sql))
>>>>>>> dbdev
    {
        SkipDecryptRes = true;
        return sql;
    }

    MemoryContextSwitchTo(MessageContext);

<<<<<<< HEAD
    List *AST = (List *)raw_parser(sql);
=======
    Timer t;
    List *AST = (List *)raw_parser(sql);

>>>>>>> dbdev
    if (AST)
        parseTree = (Node *)linitial(AST);
    else
    {
        SkipDecryptRes = true;
        errmsg("error when parse SQL:\n %s\n", sql);
    }
    if (parseTree)
    {
        MemoryContextSwitchTo(EncryptContext);
        Timer t;

        if (needSkipEncryption(nullptr, parseTree))
        {
            SkipDecryptRes = true;
            return sql;
        }
        EncryptInfo *info = (EncryptInfo *)palloc0(sizeof(EncryptInfo));
        info->sql = sql;
        encParseTree = encryptStmt(parseTree, info);
        counter->count("SQL Encryption", t.passedTimeMicroSecond());
        if (!needDecryptResult(parseTree))
        {
            SkipDecryptRes = true;
        }
        // mapperSave();
    }

    if (encParseTree)
    {
        Timer t;
        res = parseTree2str(encParseTree);
        counter->count("SQL AST To String", t.passedTimeMicroSecond());
    }

    if (!res)
        SkipDecryptRes = true;

    const char *retStuff = res ? res : sql;
#ifndef NDEBUG
<<<<<<< HEAD
    debug2file(retStuff, false);
    std::cout << retStuff << std::endl;
    std::cout << "main done\n";
#endif

=======
    pid_t pid = getpid();
    std::cout << "PID:" << pid << std::endl;
    debug2file(sql, true);
    std::cout << sql << std::endl;

    debug2file(retStuff, false);
    std::cout << retStuff << std::endl;
    std::cout << "main done\n";
#endif
>>>>>>> dbdev
    return retStuff;
}
