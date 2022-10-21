
#pragma once

#include <string>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <memory.h>
#include "config.h"
#include "LogBase.h"
// #include "UtilityFunctions.h"


// Needed to call untrusted key exchange library APIs, i.e. sgx_ra_proc_msg2.
#include "sgx_ukey_exchange.h"

// Needed to query extended epid group id.
#include "sgx_uae_service.h"

#include "sgx_uswitchless.h"

class EnclaveManager
{

public:
    static EnclaveManager *getInstance();

#ifdef USE_SGX

    virtual ~EnclaveManager();
    sgx_status_t createEnclave();
    sgx_enclave_id_t getID();
    sgx_status_t getStatus();
    sgx_ra_context_t getContext();

    void useRA() {
        m_useRA = true;
    }

    // void setEnclavePath(const string& s) {
    //     enclave_path = move(s);
    // }

private:
    EnclaveManager();
    
    string enclave_path = Config::getInstance()->Get("common.EnclavePath", "").asString();
    bool m_useRA = false;
    sgx_enclave_id_t enclave_id = -1;
    sgx_status_t status;
    sgx_ra_context_t context;
#else
    int getID() {return -1;}
#endif
};
