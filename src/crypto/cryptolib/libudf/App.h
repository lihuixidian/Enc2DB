
#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include "settings.h"
#include "EnclaveManager.h"
#ifdef USE_SGX
#include "sgx_error.h" /* int  */
#include "sgx_eid.h"   /* sgx_enclave_id_t */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif

#ifndef USE_SGX
int __attribute__((weak))  ecall_sahe_add(long eid, void *a, void *b, void *c, size_t outlen);
int __attribute__((weak))  ecall_sahe_mul(long eid, void *a, long int b, void *c, size_t outlen);
int __attribute__((weak))  ecall_sahe_sub(long eid, void *a, void *b, void *c, size_t outlen);
int __attribute__((weak))  ecall_sahe_neg(long eid, void *a, void *c, size_t outlen);
int __attribute__((weak))  ecall_smhe_mul(long eid, void *a, void *b, void *c, size_t outlen);
int __attribute__((weak))  ecall_smhe_mulp(long eid, void *a, long int b, void *c, size_t outlen);
int __attribute__((weak))  ecall_smhe_div(long eid, void *a, void *b, void *c, size_t outlen);
int __attribute__((weak))  ecall_smhe_pow(long eid, void *a, long int b, void *c, size_t outlen);
int __attribute__((weak))  ecall_smhe_inv(long eid, void *a, void *c, size_t outlen);
int __attribute__((weak))  ecall_ore_compare(long eid, void *a, void *b, int* ret);
int __attribute__((weak))  ecall_genMasterKey(long eid, const char *path);
int __attribute__((weak))  ecall_openMasterKey(long eid, const char *path, void *out, size_t outlen);
int __attribute__((weak))  ecall_encryptKeyWithEnclave(long eid, const char *key, size_t keySize, const char *path);
int __attribute__((weak))  ecall_fillLru(long eid);
int __attribute__((weak))  ecall_getLruHit(long eid, double *d);
int __attribute__((weak)) sl_init_switchless(long eid, int *retval, void *sl_data);
int __attribute__((weak)) sl_run_switchless_tworker(long eid, int *retval);

#endif
