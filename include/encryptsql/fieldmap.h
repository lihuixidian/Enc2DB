#pragma once

// #include "transform/encryptstmt.h"

<<<<<<< HEAD
#include "encryptsql/utils.h"

#define MAP_NAME_BUFFER_SIZE 128 / 8
=======
#include "utils.h"

#define MAP_NAME_BUFFER_SIZE 64 / 8
>>>>>>> dbdev
#define MAP_NAME_STR_SIZE 2 * MAP_NAME_BUFFER_SIZE + 8

const char* plainDataType2String(PlainDataType t);

void getValueBy(const char* tag, const char* key, char* val);

void setValueBy(const char* tag, const char* key, const char* v);

void setCurrentNodeOnce(const char* table, const char* schema);

void setFromTable(const char* table);

void setTableAlias(const char* tablename, const char* aliasname);

void mapperInit(const char* path);

void mapperCleanup();

void mapperSave();

void mapperDestory();

int getMapperStatus();

bool isFilteredSql(const char* sql);

bool hasCipher(const char* column, T_Cipher t);

void deleteNode(const char* table, const char* schema);

void addColumnAndType(const char* column, const char* _typename);

void getFirstTableColumnsAlloc(char*** cols, int* size);

size_t getFirstTableColumnSize();  // 获取mapper tables.back()的column数量

void getColumnType(const char* col, char* v);  // 获取指定column的type，column明文无后缀

void getColumnCiphers(const char* col, T_Cipher* ciphers, int* size); // 需要自行分配内存，获取指定col的加密方式, 并且sort

void getFirstTableAllColumnsInfoAlloc(ColumnInfo*** pinfos, int* size);

void getSpecifyColumnsInfoAlloc(List* cols, ColumnInfo*** pinfos);

ColumnInfo* getColumnInfoAlloc(const char* col);