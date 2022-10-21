#pragma once

#include <cctype>
#include <stdint.h>
#include <string.h>

struct StatusReport
{
    enum UdfKind
    {
        SAHE_ADD, // cipher + cipher
        SAHE_MUL, // cipher * plain
        SAHE_SUB, // cipher - cipher
        SAHE_NEG, // - cipher

        SMHE_MUL, // cipher * cipher
        SMHE_MULP,
        SMHE_DIV,
        SMHE_POW,
        SMHE_INV,

        ORE,
        DET,
        SUM,
    };

    enum ExecutionPath
    {
        SOFTWARE,
        HARDWARE,
        COUNT
    };

    UdfKind udf_kind;
    double runing_time;
    size_t processed_data_size;

    double enclave_memory_amout; // 运行之前的enclave内存容量

};
