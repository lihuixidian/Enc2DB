

#ifdef __cplusplus
extern "C"
{
#endif

<<<<<<< HEAD:src/jsoncpp/fieldmap.cpp
#include "encryptsql/combine.h"
=======
#include "postgres.h" // 与 #include <iomanip> 冲突

#include "utils/palloc.h"
#include "utils/memutils.h"
>>>>>>> dbdev:src/encryptsql/fieldmap.cpp

#include "utils/memutils.h"
#include "nodes/pg_list.h"

#ifdef __cplusplus
}
#endif
#include "encryptsql/fieldmap.h"
<<<<<<< HEAD:src/jsoncpp/fieldmap.cpp
#include "encryptsql/utils.h"
=======
#include "utils.h"
>>>>>>> dbdev:src/encryptsql/fieldmap.cpp

#include <json/json.h>

#include <string>
#include <fstream>

#include <vector>
#include <set>
#include <memory>
#include <algorithm>
#include <iostream>
#include <mutex>
#include "crypto/crypto.h"
// using namespace std;

using std::cout;
using std::endl;
using std::make_shared;
using std::mutex;
using std::shared_ptr;
using std::string;
using std::transform;
using std::vector;

#define SHARED(x) shared_ptr<Json::Value>(x)
#define MAKE_SHARED(x) make_shared<Json::Value>(x)

// typedef enum {
//     SET_ONCE,
//     SET_FROM_TABLE,
//     SET_UNSET

// } CurrentNodeStatus;

class FieldMapper
{
private:
    shared_ptr<Json::Value> root;
    string path;
    mutable Json::Value *currentSchema = nullptr;
    mutable Json::Value *currentTable = nullptr;
    Json::Value schemaDefaultValue;
    Json::Value tableDefaultValue;
    Json::Value columnDefaultValue;
    int status;

    enum
    {
        SET_ONCE,
        SET_FROM_TABLE,
        SET_UNSET
    };

    std::vector<string> tables;

public:
    FieldMapper();
    FieldMapper(const string &path);
    ~FieldMapper();
    Json::Value *getCurNodeBy(const string &tag, const string &key);
    string getValueBy(const string &tag, const string &key);
    void setValueBy(const string &tag, const string &key, const string &value);
    void setCurrentNodeOnce(string table = "", string schema = "");
    void setFromTable(const string &table);
    void setDefaultValue();
    void init();
    void setSysMapRelation();
    void setTableAlias(const string &tablename, const string &aliasname);
    string tableAliasTransform(string tablename);
    void cleanup();
    void save(string _path = "");
    void setFilteredSql(string sql);
    bool isFilteredSql(string sql) const;
    bool hasCipher(const string &column, T_Cipher t);
    int getStatus() const
    {
        return status;
    }
    void deleteNode(string table = "", string schema = "");
    void addColumnAndType(const string &column, const string &_typename);
    void getTableColumns(char **cols, string table = "");
    void getColumnType(const string &col, char *v);
    size_t getTableColumnSize(string table = "") const;
    void setDefaultUDFMap();
    vector<T_Cipher> getColumnCipherArray(const string &column);
};

void FieldMapper::setDefaultUDFMap()
{
}

size_t FieldMapper::getTableColumnSize(string table) const
{
    auto oldTable = currentTable;
    if (table.size())
    {
        currentTable = &currentSchema->operator[](table);
    }

    currentTable = oldTable;

    return currentTable->operator[]("columns").size();
}

void FieldMapper::addColumnAndType(const string &column, const string &_typename)
{
    auto table = tables.back();
    currentTable->operator[]("columns").append(column);
    currentTable->operator[](column)["type"] = _typename;
}

string FieldMapper::tableAliasTransform(string tablename)
{
    return currentSchema->isMember(tablename) ? tablename : (*root)["tmp"]["tbltmp"]["alias"][tablename].asString();
}

FieldMapper::FieldMapper()
{
}

void FieldMapper::setDefaultValue()
{
    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    const char baseContext[] = " {\"map\": {} } ";
    reader->parse(baseContext, baseContext + sizeof(baseContext), &tableDefaultValue, nullptr);
    reader->parse(baseContext, baseContext + sizeof(baseContext), &schemaDefaultValue, nullptr);
    const char baseContext2[] = " {\"cipher\":[]} ";
    reader->parse(baseContext2, baseContext2 + sizeof(baseContext2), &columnDefaultValue, nullptr);

    if (root->operator[]("udffunc").isNull() || (*root)["udffunc"]["map"].isNull())
    {
        const char baseContext3[] = "{           \
                \"map\" : {                     \
                    \"*\" : \"smhe_multiply\",    \
                    \"+\" : \"sahe_add\",    \
                    \"-\" : \"sahe_sbustract\",    \
                    \"/\" : \"smhe_divide\",    \
                    \"<\" : \"udf_ore_lt\",     \
                    \"<=\" : \"udf_ore_le\",    \
                    \">\" : \"udf_ore_gt\",     \
                    \">=\" : \"udf_ore_ge\",    \
                    \"add\" : \"sahe_add\",  \
                    \"avg\" : \"avg\",          \
                    \"count\" : \"count\",      \
                    \"date_part\" : \"date_part\",  \
                    \"pow\" : \"smhe_pow\",  \
                    \"min\" : \"ore_min\",  \
                    \"max\" : \"ore_max\",  \
                    \"sum\" : \"sahe_sum\" }}}";
        reader->parse(baseContext3, baseContext3 + sizeof(baseContext3), &root->operator[]("udffunc"), nullptr);
    }
}

void FieldMapper::setSysMapRelation()
{
    // 如果没有默认的系统表映射，则设置此映射

    ////////////////////////////////////////////////////////////////
    ///// Table pg_settings
    // setValueBy("table", "pg_settings", "pg_settings");
    // setFromTable("pg_settings");
    // setValueBy("column", "name", "name");
    // setValueBy("column", "setting", "setting");
    // setValueBy("column", "setting", "setting");

    // 设置一些过滤SQL
    setFilteredSql("select name, setting from pg_settings where name in ('connection_info')");
    setFilteredSql("select pg_backend_pid();");
    setFilteredSql("SELECT VERSION()");
    cleanup();
    // save();
}

void FieldMapper::init()
{

    // std::ifstream fin;
    std::fstream fin(path, std::fstream::in | std::fstream::out);
    if (fin.is_open())
    {
        Json::CharReaderBuilder builder;
        builder["collectComments"] = true;
        JSONCPP_STRING errs;
        if (!fin.eof() && !parseFromStream(builder, fin, root.get(), &errs))
            std::cout << errs << std::endl;
    }
    (*root)["tmp"]["restmp"] = {};
    (*root)["tmp"]["tbltmp"] = {};

    if ((*root)["schema"]["public"].isNull())
    {
        (*root)["schema"]["public"] = Json::Value(schemaDefaultValue);
    }
    

    setSysMapRelation();
    setDefaultValue();
    currentSchema = &(*root)["schema"]["public"];
    fin.close();
    // save();
<<<<<<< HEAD:src/jsoncpp/fieldmap.cpp
    
=======
>>>>>>> dbdev:src/encryptsql/fieldmap.cpp
}

FieldMapper::FieldMapper(const string &_path)
{
    path = _path;
    root = MAKE_SHARED();
    init();
}

void FieldMapper::save(string _path)
{
    // 先删除tmp节点，写完文件后再回复。

    auto tmp = (*root)["tmp"];
    root->removeMember("tmp");
    std::ofstream fout;
    if (_path.empty())
        fout.open(path);
    else
        fout.open(_path);

    if (!fout.is_open())
    {
        //" << path << "open failed\n";
    }
    else
    {
        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(*root, &fout);
        fout << std::endl; // add lf and flush
    }
    fout.close();
    (*root)["tmp"] = tmp; // 不希望保存tmp下的数据，但是在解析一次SQL时可能调用save, 这样就会丢失别名数据.
}

FieldMapper::~FieldMapper()
{
    // mapper...\n";
    save();
}

Json::Value *FieldMapper::getCurNodeBy(const string &tag, const string &key)
{
    // if (currentSchema && currentSchema->isNull()) {
    //     errmsg("node currentSchema is null\n");
    //     return nullptr;
    // }
    Json::Value *currentNode = nullptr;
    if (!tag.compare("table"))
        currentNode = currentSchema;
    else if (!tag.compare("column"))
    {
        if (status == SET_ONCE)
        {
            currentTable = &(*currentSchema)[tableAliasTransform(tables.back())];
            tables.pop_back();
            status = SET_FROM_TABLE; // 用过一次之后就变为Set from table
        }
        else if (status == SET_FROM_TABLE)
        {
            if (tables.size() == 1)
            {
                // 可能来自 create 只有一个table，就不需要搜索了
                currentTable = &(*currentSchema)[tableAliasTransform(tables.back())];
            }
            else
            {
                for (auto it : tables)
                {
                    auto transfromed = tableAliasTransform(it);
                    if (!(*currentSchema)[transfromed]["map"][key].isNull() ||
                        !(*currentSchema)[transfromed][key].isNull())
                    {
                        currentTable = (&(*currentSchema)[it]);
                        break;
                    }
                    else
                    {
                        (*currentSchema)[transfromed]["map"].removeMember(key);
                        (*currentSchema)[transfromed].removeMember(key);
                    }
                }
            }
        }
        else
        {
            // errmsg("Unknown mapper status: %d\n", status);
        }
        currentNode = currentTable;
    }
    else if (!tag.compare("restmp"))
        currentNode = &(*root)["tmp"]["restmp"];
    else if (!tag.compare("tbltmp"))
        currentNode = &(*root)["tmp"]["tbltmp"];
    else if (!tag.compare("udffunc"))
        currentNode = &(*root)["udffunc"];
    else
        currentNode = &(*root)["schema"];
    return currentNode;
}

string FieldMapper::getValueBy(const string &tag, const string &key)
{
    // 根据tag不同，到对应的字段中寻找map
    auto currentNode = getCurNodeBy(tag, key);
    if (!currentNode)
        return (*root)["tmp"]["restmp"]["map"][key].isNull() ? "" : (*root)["tmp"]["restmp"]["map"][key].asString();
    auto res = (*currentNode)["map"][key];
    if (res.isNull())
    { // 处理别名的情况
        (*currentNode)["map"].removeMember(key);
        // 在 targetList中，无法分辨target name是列名还是别名
        if (!tag.compare("table"))
            // 可能是table的别名
            res = (*root)["tmp"]["tbltmp"]["map"][key];
        else if (!tag.compare("column"))
        {
            // auto colname = key.substr(0, key.find_last_of("_")); ???
            res = (*root)["tmp"]["restmp"]["map"][key];
        }
    }
    return res.isNull() ? "" : res.asString();
}

void FieldMapper::setValueBy(const string &tag, const string &key, const string &v)
{
    // 可以多次设置， 自动覆盖原来的值
    // bidirection map
    if (!tag.compare("restmp"))
    {
        (*root)["tmp"]["restmp"]["map"][key] = v;
        (*root)["tmp"]["restmp"]["map"][v] = key;
        return;
    }

    auto currentNode = getCurNodeBy(tag, key);
    auto old_v = (*currentNode)["map"][key];
    if (!old_v.isNull()) // 删除原来的值
        (*currentNode)["map"].removeMember(old_v.asString());
    (*currentNode)["map"][key] = v;
    (*currentNode)["map"][v] = key;

    auto delimiterIndex = key.find_last_of("_");
    string colname = key, suffix = "";
    if (delimiterIndex != key.npos)
    {
        colname = key.substr(0, delimiterIndex);
        suffix = key.substr(delimiterIndex + 1, key.size() - delimiterIndex - 1);
    }
    if (!tag.compare("table"))
        (*currentNode)[key] = Json::Value(tableDefaultValue);

    else if (!tag.compare("column"))
    {
        int cipher = string2TypeCipher(suffix.c_str());

        if ((*currentNode)[colname].isNull())
        { //如果是空的，就新建一个column，如果不是就进行append
            (*currentNode)[colname] = Json::Value(columnDefaultValue);
<<<<<<< HEAD:src/jsoncpp/fieldmap.cpp
            if (cipher != CIPHER_NOCRYPT)
                (*currentNode)[colname]["cipher"].append(cipher);
=======
            // if (cipher != CIPHER_NOCRYPT)
            (*currentNode)[colname]["cipher"].append(cipher);
>>>>>>> dbdev:src/encryptsql/fieldmap.cpp
        }
        else // 往cipherArray 中添加cipher
        {
            auto cipherArray = (*currentNode)[colname]["cipher"];
            auto it = cipherArray.begin();
            for (; it != cipherArray.end(); ++it)
            {
                if (it->asInt() == cipher)
                    break;
            }
            if (it == cipherArray.end())
                (*currentNode)[colname]["cipher"].append(cipher);
        }
    }
    else if (!tag.compare("schema"))
        (*currentNode)[key] = Json::Value(schemaDefaultValue);
}

void FieldMapper::setCurrentNodeOnce(string table, string schema)
{
    status = SET_ONCE;
    if (!schema.empty())
    {
        if ((*root)["schema"][schema].isNull())
            (*root)["schema"][schema] = Json::Value(schemaDefaultValue);
        currentSchema = &(*root)["schema"][schema];
    }
    if (!table.empty())
        tables.push_back(table);
}

void FieldMapper::deleteNode(string table, string schema)
{
    if (!schema.empty())
    {
        (*root)["schema"].removeMember(schema);
        auto v = (*root)["schema"]["map"][schema].asString();
        (*root)["schema"]["map"].removeMember(schema);
        (*root)["schema"]["map"].removeMember(v);
    }

    if (!table.empty())
    {
        (*currentSchema).removeMember(table);
        auto v = (*currentSchema)["map"][table].asString();
        (*currentSchema)["map"].removeMember(table);
        (*currentSchema)["map"].removeMember(v);
    }
    cleanup();
    // save();
}

void FieldMapper::setFromTable(const string &table)
{
    status = SET_FROM_TABLE;
    tables.push_back(table);
    currentTable = &currentSchema->operator[](table);
}

void FieldMapper::setTableAlias(const string &tablename, const string &aliasname)
{
    (*root)["tmp"]["tbltmp"]["alias"][tablename] = aliasname;
    (*root)["tmp"]["tbltmp"]["alias"][aliasname] = tablename;
}

void FieldMapper::cleanup()
{
    tables.clear();
    status = SET_UNSET;
    currentTable = nullptr;
    currentSchema = &(*root)["schema"]["public"];
}

void FieldMapper::setFilteredSql(string sql)
{
    transform(sql.begin(), sql.end(), sql.begin(), tolower);
    (*root)["filteredSql"][sql] = 1;
}

bool FieldMapper::isFilteredSql(string sql) const
{
    transform(sql.begin(), sql.end(), sql.begin(), tolower);
    if ((*root)["filteredSql"][sql].isNull())
    {
        (*root)["filteredSql"].removeMember(sql);
        return false;
    }
    return true;
}

bool FieldMapper::hasCipher(const string &column, T_Cipher t)
{
    vector<T_Cipher> &&ciphers = getColumnCipherArray(column);
    for (auto &it : ciphers)
    {
        if (it == t)
            return true;
    }
    return false;
};

vector<T_Cipher> FieldMapper::getColumnCipherArray(const string &column)
{
    string &topTable = tables.back(); // 先记录下栈顶的表
    auto statusOld = status;
    auto currentNode = *getCurNodeBy("column", column);
    auto cipherArray = currentNode[column]["cipher"];
    if (statusOld == SET_ONCE)
        setCurrentNodeOnce(topTable);
    vector<T_Cipher> vectorCipherArray;
    vectorCipherArray.resize(cipherArray.size());

    for (int i = 0; i < cipherArray.size(); ++i)
        vectorCipherArray[i] = (T_Cipher)cipherArray[i].asInt();

    return vectorCipherArray;
}

void FieldMapper::getTableColumns(char **cols, string table)
{

    auto oldTable = currentTable;

    if (table.size())
        currentTable = &currentSchema->operator[](table);

    for (int i = 0; i < currentTable->operator[]("columns").size(); i++)
    {
        auto columns = currentTable->operator[]("columns");
        auto column = columns[i].asCString();
        strcpy(cols[i], column);
    }

    currentTable = oldTable;
}

void FieldMapper::getColumnType(const string &col, char *v)
{
    auto curNode = getCurNodeBy("column", col);
    if (!curNode)
    {
        v[0] = '\0';
        return;
    }
    auto res = curNode->operator[](col)["type"];
    if (res.isNull())
        return;
    auto type = res.asCString();
    strcpy(v, type);
}

static thread_local std::unique_ptr<FieldMapper> mapper; // 线程局部存储

static std::mutex save_mutex;
////////////////////////////////////////
//// C API Wrapper
////////////////////////////////////////

int getMapperStatus()
{
    return mapper->getStatus();
}

void initCheck()
{
    if (!mapper)
    {
        cout << "mapper is not init\n";
        exit(-1);
    }
}

void mapperSave()
{
    {
<<<<<<< HEAD:src/jsoncpp/fieldmap.cpp
        std::unique_lock<std::mutex>(save_mutex);           //TODO: 尝试优化
=======
        std::unique_lock<std::mutex>(save_mutex); //TODO: 尝试优化
>>>>>>> dbdev:src/encryptsql/fieldmap.cpp
        mapper->save();
    }
}

void getValueBy(const char *tag, const char *key, char *val)
{
    initCheck();

    string res = mapper->getValueBy(tag, key);

    sprintf(val, "%s", res.c_str());
}

void setValueBy(const char *tag, const char *key, const char *v)
{
    initCheck();
    mapper->setValueBy(tag, key, v);
}

void setCurrentNodeOnce(const char *table, const char *schema)
{
    mapper->setCurrentNodeOnce(table, schema);
}

void setFromTable(const char *table)
{
    mapper->setFromTable(table);
}

void setTableAlias(const char *tablename, const char *aliasname)
{
    mapper->setTableAlias(tablename, aliasname);
}

void mapperInit(const char *path)
{
    mapper.reset(new FieldMapper(path));
}

void mapperCleanup()
{
    mapper->cleanup();
}

<<<<<<< HEAD:src/jsoncpp/fieldmap.cpp
// void mapperDestory()
// {
//     delete mapper;
// }

=======
>>>>>>> dbdev:src/encryptsql/fieldmap.cpp
bool hasCipher(const char *column, T_Cipher t)
{
    return mapper->hasCipher(column, t);
}

const char *plainDataType2String(PlainDataType t)
{
    switch (t)
    {
    case T_STRING_CATALOG:
        return "catalog";
    case T_STRING_TABLE:
        return "table";
    case T_STRING_COLUMN:
        return "column";
    case T_STRING_SCHEMA:
        return "schema";
    case T_STRING_UDF_FUNCTION:
        return "udffunc";
    case T_STRING_RESTMP:
        return "restmp";
    case T_STRING_TBLTMP:
        return "tbltmp";
    default:
        return "others";
    }
}

bool isFilteredSql(const char *sql)
{
    return mapper->isFilteredSql(sql);
}

void deleteNode(const char *table, const char *schema)
{
    mapper->deleteNode(table, schema);
}

void addColumnAndType(const char *column, const char *_typename)
{
    mapper->addColumnAndType(column, _typename);
}

void getFirstTableColumnsAlloc(char ***cols, int *size) // 此函数自动分配内存， 获取指定table的所有columns
{
    *size = mapper->getTableColumnSize();

    *cols = (char **)palloc(sizeof(char *) * *size);
    for (size_t i = 0; i < *size; i++)
        (*cols)[i] = (char *)palloc0(Short_Buffer_Size);
    mapper->getTableColumns(*cols, "");
}

void getColumnType(const char *col, char *v) // 获取指定col的type
{
    mapper->getColumnType(col, v);
}

void getColumnCiphers(const char *col, T_Cipher *ciphers, int *size) // 需要自行分配内存，获取指定col的加密方式, 并且sort
{
    auto vectCiphers = mapper->getColumnCipherArray(col);
    *size = vectCiphers.size();
    for (size_t i = 0; i < *size; i++)
        ciphers[i] = vectCiphers[i];
    std::sort(ciphers, ciphers + *size);
}

ColumnInfo *getColumnInfoAlloc(const char *col) // 内存自动分配，获取指定列的信息
{
    ColumnInfo *cinfo = (ColumnInfo *)palloc(sizeof(ColumnInfo));
    cinfo->type = (char *)palloc(Short_Buffer_Size);
    cinfo->name = (char *)palloc(Short_Buffer_Size);
    strcpy(cinfo->name, col);
    getColumnType(col, cinfo->type);
    T_Cipher ciphers[CIPHER_COUNT];
    int nCipher = 0;
    getColumnCiphers(col, ciphers, &nCipher);
    cinfo->ciphers = (T_Cipher *)palloc(sizeof(T_Cipher) * nCipher);
    cinfo->len = nCipher;
    std::copy(ciphers, ciphers + nCipher, cinfo->ciphers);

    return cinfo;
}

void getFirstTableAllColumnsInfoAlloc(ColumnInfo ***pinfos, int *size) // 内存自动分配，获取所有的指定table(默认最后入栈的table下)的所有的
{
    char **cols;
    int nColumn = 0;
    getFirstTableColumnsAlloc(&cols, &nColumn);

    if (nColumn)
    {
        ColumnInfo **infos = (ColumnInfo **)palloc(sizeof(ColumnInfo *) * nColumn);
        *pinfos = infos;
        for (size_t i = 0; i < nColumn; i++)
        {
            char *col = cols[i];
            infos[i] = getColumnInfoAlloc(col);
        }
    }
    *size = nColumn;
}

void getSpecifyColumnsInfoAlloc(List *cols, ColumnInfo ***pinfos)
{ // 内存自动分配, 获取指定列的信息.
    ListCell *cell;
    ColumnInfo **infos = (ColumnInfo **)palloc(sizeof(ColumnInfo *) * cols->length);
    *pinfos = infos;

    int i = 0;
    foreach (cell, cols)
    {
        infos[i++] = getColumnInfoAlloc(strVal((Value *)lfirst(cell)));
    }
}

size_t getFirstTableColumnSize()
{
    return mapper->getTableColumnSize();
}
