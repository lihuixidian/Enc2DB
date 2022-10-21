

#ifdef __cplusplus
extern "C"
{
#endif
#include "postgres.h"
#ifdef __cplusplus
}
#endif
#include "crypto/crypto.h"
#include "encryptsql/fieldmap.h"
#include "encryptsql.h"

#include <string>
#include <vector>
#include <string>
#include <memory>

#include "utils.h"
#include "TimeCounter.h"
#include "Singleton.h"

extern bool Encrypt;
extern bool SkipDecryptRes;

using std::shared_ptr;

extern "C" void decryptResult(int numberAttr, int numTuples, pAttrDescs pattDescs, pTuples ptuples)
    __attribute__((visibility("default")));

void decryptResult(int numberAttr, int numTuples, pAttrDescs pattDescs, pTuples ptuples)
{
    using namespace std;
    if (!Encrypt || SkipDecryptRes)
        return;

#ifndef NDEBUG
    // std::cout << "begin to decrypt: " << numberAttr << endl;
    // std::cout << retStuff << std::endl;
#endif

    TimeCounter *counter = Singleton<TimeCounter>::instance();
    Timer timer;
    PGresAttValue **tuples = (PGresAttValue **)ptuples;
    PGresAttDesc *attDescs = (PGresAttDesc *)pattDescs;
    for (int j = 0; j < numberAttr; ++j)
    {
        string cipherColName = attDescs[j].name;
        size_t oldAttSize = cipherColName.size();
        // int cipherColnameSZ = attDescs[i].name;
        string colnameSubfix = getMappedName(T_STRING_COLUMN, cipherColName.c_str(), NULL);
        string colname = colnameSubfix, subfix;
        bool unnamedFlag = false;

        auto ccit = cipherColName.find_first_of("_");

        if (ccit != cipherColName.npos)
        {
            auto prefix = cipherColName.substr(0, ccit);
            if (!prefix.compare("restmp"))
                unnamedFlag = true;
        }

        if (!strcmp(cipherColName.c_str(), "?column?"))
        {
            subfix = "AES";
            colname = "unnamedCol";
            unnamedFlag = true;
        }
        else
        {
            auto it = colnameSubfix.find_last_of("_");
            if (it != colnameSubfix.npos)
            {
                colname = colnameSubfix.substr(0, it);
                subfix = colnameSubfix.substr(it + 1, colnameSubfix.size() - it);
            }
        }

        if (string2TypeCipher(subfix.c_str()) == CIPHER_COUNT) //别名中出现了下划线的情况。
            colname = colnameSubfix;
        strcpy(attDescs[j].name, colname.c_str()); // WARN
        if (strlen(attDescs[j].name) > oldAttSize)
            abort();
        shared_ptr<char> coltypename(new char[32](), [](char *p) { delete[] p; });
        getColumnType(colname.c_str(), coltypename.get());
        if (!*coltypename && !unnamedFlag)
        { // 这个column不在记录当中
            continue;
        }
        COLUMN_TYPE ctype = TYPE_STRING;
        if (!unnamedFlag)
            ctype = unifyColumnType(coltypename.get());
        else
            ctype = TYPE_INTEGER;

        void (*anyTypetoString)(const char *, char *const, void *) = NULL; // 把buffer转为相应类型的数据。
        switch (ctype)
        {
        case TYPE_FLOAT:
            anyTypetoString = &FloatTypetoString;
            break;

        case TYPE_STRING:

            anyTypetoString = NULL; //无需转换
            break;
        case TYPE_INTEGER:
            anyTypetoString = &IntTypetoString;
            break;
        }

        if (!subfix.empty())
        {
            T_Cipher t = string2TypeCipher(subfix.c_str());
            if (t == CIPHER_NOCRYPT)
                continue;
            for (int i = 0; i < numTuples; ++i)
            {
                size_t buffSZ = tuples[i][j].len / 2 + 1; // 密文buffer长度， 应该大于hexstr长度的一半

                if (tuples[i][j].len == -1) // 数据列为Null
                    continue;

                size_t oldDataLen = tuples[i][j].len;

                size_t plainBuffSZ = buffSZ;
                uint8_t *plainBuff = nullptr;
                uint8_t *pbuff = nullptr;
                shared_ptr<uint8_t> buff(new uint8_t[buffSZ](), [](uint8_t *p) { delete[] p; });
                if (t != CIPHER_SAHE && t != CIPHER_SMHE)  // true 为采用encode的加密列
                {
                    hex2buf(buff.get(), &buffSZ, tuples[i][j].value, tuples[i][j].len); // 若要改变编码方式，主要换掉这个函
                    pbuff = buff.get();
                }
                else {
                    pbuff = (uint8_t *)tuples[i][j].value;
                    buffSZ = tuples[i][j].len + 1;
                }
                // 当前这列的密文为float类型，那么就需要转换
                if((t == CIPHER_SAHE || t == CIPHER_SMHE) && (pbuff[strlen((char*)pbuff) - 1] == '2' && anyTypetoString != FloatTypetoString)) {
                    anyTypetoString = FloatTypetoString;
                }
                // if(CIPHER_NOCRYPT == t)
                //     buffSZ = tuples[i][j].len;
                plainBuff = encryptValue(t, pbuff, buffSZ, &plainBuffSZ, false);
                if(anyTypetoString) {
                    anyTypetoString((const char *)plainBuff, tuples[i][j].value, (void *)coltypename.get());
                }
                else {
                    memcpy(tuples[i][j].value, (const char *)plainBuff, min(strlen((const char *)plainBuff) + 1, plainBuffSZ));
                }
                // shared_ptr<char> res = new char[plainBuffSZ]();
                // if (ctype != TYPE_STRING) {
                //     if(pbuff[buffSZ-2] == '2') {    // 此密文是一个浮点数，需要除scale
                //         FloatTypetoString((const char *)plainBuff, tuples[i][j].value, (void *)coltypename.get());
                //     }
                //     else {
                //         IntTypetoString((const char *)plainBuff, tuples[i][j].value, (void *)coltypename.get());
                //     }
                // }
                // else {
                //     memcpy(tuples[i][j].value, (const char *)plainBuff, min(strlen((const char *)plainBuff) + 1, plainBuffSZ));
                // }
                // strcpy(tuples[i][j].value, (char*)plainBuff);
                // strncpy(tuples[i][j].value, res, min(strlen(res), tuples[i][j].len) + 1);
                tuples[i][j].len = strlen((char *)tuples[i][j].value);
                if (tuples[i][j].len > oldDataLen)
                    abort(); // 出现了内存越界
                // clean
                // delete[] buff;
                // delete[] res;
            }
        }
    }
#ifndef NDEBUG
    // std::cout << "decrypt done\n";
    // std::cout << retStuff << std::endl;
#endif

    counter->count("SQL Decryption", timer.passedTimeMicroSecond());

    cleanup();
}