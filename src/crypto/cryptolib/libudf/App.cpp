#include "errmsgBridge.h"

#ifdef USE_SGX

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <pwd.h>
#define MAX_PATH FILENAME_MAX

#include <sgx_urts.h>
#include <sgx_uswitchless.h>

#include "App.h"


#include "Singleton.h"
#include "routineDecider.h"
#include "Enclave_u.h"


extern "C" {
#include "postgres.h"

}


/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;



typedef struct _sgx_errlist_t
{
    sgx_status_t err;
    const char *msg;
    const char *sug; /* Suggestion */
} sgx_errlist_t;

/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] = {
    {SGX_ERROR_UNEXPECTED,
     "SGX: Unexpected error occurred.",
     NULL},
    {SGX_ERROR_INVALID_PARAMETER,
     "SGX: Invalid parameter.",
     NULL},
    {SGX_ERROR_OUT_OF_MEMORY,
     "SGX: Out of memory.",
     NULL},
    {SGX_ERROR_ENCLAVE_LOST,
     "SGX: Power transition occurred.",
     "SGX: Please refer to the sample \"PowerTransition\" for details."},
    {SGX_ERROR_INVALID_ENCLAVE,
     "SGX: Invalid enclave image.",
     NULL},
    {SGX_ERROR_INVALID_ENCLAVE_ID,
     "SGX: Invalid enclave identification.",
     NULL},
    {SGX_ERROR_INVALID_SIGNATURE,
     "SGX: Invalid enclave signature.",
     NULL},
    {SGX_ERROR_OUT_OF_EPC,
     "SGX: Out of EPC memory.",
     NULL},
    {SGX_ERROR_NO_DEVICE,
     "SGX: Invalid SGX device.",
     "SGX: Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."},
    {SGX_ERROR_MEMORY_MAP_CONFLICT,
     "SGX: Memory map conflicted.",
     NULL},
    {SGX_ERROR_INVALID_METADATA,
     "SGX: Invalid enclave metadata.",
     NULL},
    {SGX_ERROR_DEVICE_BUSY,
     "SGX: SGX device was busy.",
     NULL},
    {SGX_ERROR_INVALID_VERSION,
     "SGX: Enclave version was invalid.",
     NULL},
    {SGX_ERROR_INVALID_ATTRIBUTE,
     "SGX: Enclave was not authorized.",
     NULL},
    {SGX_ERROR_ENCLAVE_FILE_ACCESS,
     "SGX: Can't open enclave file.",
     NULL},
    {SGX_ERROR_NDEBUG_ENCLAVE,
     "SGX: The enclave is signed as product enclave, and can not be created as debuggable enclave. ",
     NULL}};

/* Check error conditions for loading enclave */
const char* get_error_message(sgx_status_t ret)
{
    size_t idx = 0;
    size_t ttl = sizeof sgx_errlist / sizeof sgx_errlist[0];

    for (idx = 0; idx < ttl; idx++)
    {
        if (ret == sgx_errlist[idx].err)
        {
            // if (NULL != sgx_errlist[idx].sug)
            //     printf("Info: %s\n", sgx_errlist[idx].sug);
            return sgx_errlist[idx].msg;
            break;
        }
    }
    char *buf = new char[256]();
    sprintf(buf, "Error code is 0x%X. Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n", ret);
    return buf;
}


#else
#include "App.h"

#endif

#include "errmsgBridge.h"
#include "LogBase.h"
void ocall_ereport(const char *str)
{
     util::Log(str, util::log::error);
    
}
