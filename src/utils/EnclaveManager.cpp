


#include "EnclaveManager.h"

#include <iostream>
#include <sstream>

using namespace std;
using namespace util;
#ifdef USE_SGX
typedef struct _sgx_errlist_t
{
    sgx_status_t err;
    const char *msg;
    const char *sug; /* Suggestion */
} sgx_errlist_t;

/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] = {
    {SGX_ERROR_UNEXPECTED,
     "Unexpected error occurred.",
     NULL},
    {SGX_ERROR_INVALID_PARAMETER,
     "Invalid parameter.",
     NULL},
    {SGX_ERROR_OUT_OF_MEMORY,
     "Out of memory.",
     NULL},
    {SGX_ERROR_ENCLAVE_LOST,
     "Power transition occurred.",
     "Please refer to the sample \"PowerTransition\" for details."},
    {SGX_ERROR_INVALID_ENCLAVE,
     "Invalid enclave image.",
     NULL},
    {SGX_ERROR_INVALID_ENCLAVE_ID,
     "Invalid enclave identification.",
     NULL},
    {SGX_ERROR_INVALID_SIGNATURE,
     "Invalid enclave signature.",
     NULL},
    {SGX_ERROR_OUT_OF_EPC,
     "Out of EPC memory.",
     NULL},
    {SGX_ERROR_NO_DEVICE,
     "Invalid SGX device.",
     "Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."},
    {SGX_ERROR_MEMORY_MAP_CONFLICT,
     "Memory map conflicted.",
     NULL},
    {SGX_ERROR_INVALID_METADATA,
     "Invalid enclave metadata.",
     NULL},
    {SGX_ERROR_DEVICE_BUSY,
     "SGX device was busy.",
     NULL},
    {SGX_ERROR_INVALID_VERSION,
     "Enclave version was invalid.",
     NULL},
    {SGX_ERROR_INVALID_ATTRIBUTE,
     "Enclave was not authorized.",
     NULL},
    {SGX_ERROR_ENCLAVE_FILE_ACCESS,
     "Can't open enclave file.",
     NULL},
};

/* Check error conditions for loading enclave */
string get_error_message(sgx_status_t ret)
{
    size_t idx = 0;
    size_t ttl = sizeof sgx_errlist / sizeof sgx_errlist[0];
    stringstream ss;
    for (idx = 0; idx < ttl; idx++)
    {
        if (ret == sgx_errlist[idx].err)
        {
            if (NULL != sgx_errlist[idx].sug) {
                ss << "Info: " << string(sgx_errlist[idx].sug);
            }
            ss <<  "Error: " << string(sgx_errlist[idx].msg);
            break;
        }
    }

    if (idx == ttl) {
        ss <<  "Error code is 0x" << ret << ". Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n";
    }
    return ss.str();
}

EnclaveManager::EnclaveManager() {
    createEnclave();
}

EnclaveManager *EnclaveManager::getInstance()
{
    static EnclaveManager instance;
    
    return &instance;
}

EnclaveManager::~EnclaveManager()
{
    int ret = -1;
    //TODO: 暂时去掉RA相关的代码
    // if (INT_MAX != context && m_useRA)
    // {
    //     int ret_save = -1;
    //     ret = enclave_ra_close(enclave_id, &status, context);
    //     if (SGX_SUCCESS != ret || status)
    //     {
    //         ret = -1;
    //         util::Log("Error, call enclave_ra_close fail", log::error);
    //     }
    //     else
    //     {
    //         // enclave_ra_close was successful, let's restore the value that
    //         // led us to this point in the code.
    //         ret = ret_save;
    //     }

    //     util::Log("Call enclave_ra_close success");
    // }

    sgx_destroy_enclave(enclave_id);
}

sgx_status_t EnclaveManager::createEnclave()
{
    sgx_status_t ret;
    int enclave_lost_retry_time = 1;

    do
    {
#ifdef USE_SWITCHLESS
        sgx_uswitchless_config_t us_config = SGX_USWITCHLESS_CONFIG_INITIALIZER;
        us_config.num_uworkers = 20;
        us_config.num_tworkers = 25;
        // sgx_uswitchless_config_t us_config = {0, 5, 10, 100000, 100000, {0}};
        void *enclave_ex_p[32] = {0};
        enclave_ex_p[SGX_CREATE_ENCLAVE_EX_SWITCHLESS_BIT_IDX] = &us_config;
        ret = sgx_create_enclave_ex(this->enclave_path.c_str(), SGX_DEBUG_FLAG, NULL, NULL, &this->enclave_id, NULL, SGX_CREATE_ENCLAVE_EX_SWITCHLESS, (const void **)enclave_ex_p);

#else
        ret = sgx_create_enclave(this->enclave_path.c_str(),
                                 SGX_DEBUG_FLAG,
                                 NULL,
                                 NULL,
                                 &this->enclave_id, NULL);
#endif
        if (SGX_SUCCESS != ret)
        {
            Log("Error, call sgx_create_enclave fail", log::error);
            Log(get_error_message(ret).c_str(), log::error);
            break;
        }
        else
        {
            Log("Call sgx_create_enclave success");
            // if(m_useRA) {
            //     ret = enclave_init_ra(this->enclave_id,
            //                           &this->status,
            //                           false,
            //                           &this->context);
            // }
            
        }

    } while (SGX_ERROR_ENCLAVE_LOST == ret && enclave_lost_retry_time--);

    if (ret == SGX_SUCCESS)
        util::Log("Enclave created, ID: %llx", this->enclave_id);

    return ret;
}

sgx_enclave_id_t EnclaveManager::getID()
{
    return this->enclave_id;
}

sgx_status_t EnclaveManager::getStatus()
{
    return this->status;
}

sgx_ra_context_t EnclaveManager::getContext()
{
    return this->context;
}
#else 
EnclaveManager *EnclaveManager::getInstance()
{
    static EnclaveManager instance;
    
    return &instance;
}
#endif