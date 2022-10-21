#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <string>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>

#include "EnclaveManager.h"
#include "NetworkManagerServer.h"

#include "UtilityFunctions.h"

#include "LogBase.h"
#include "config.h"


#ifdef USE_SGX
#include "remote_attestation_result.h"
#include "RAMessages.pb.h"
#endif

using namespace std;
using namespace util;

class MessageHandler
{

public:
    MessageHandler(int port = Config::getInstance()->Get("RA.port", 222).asInt());
    virtual ~MessageHandler();
    int init();
    void start();
    vector<string> incomingHandler(string v, int type);

#ifdef USE_SGX

    sgx_ra_msg3_t *getMSG3();

private:
    sgx_status_t initEnclave();
    uint32_t getExtendedEPID_GID(uint32_t *extended_epid_group_id);
    sgx_status_t getEnclaveStatus();

    void assembleAttestationMSG(RAMessages::AttestationMessage msg, ra_samp_response_header_t **pp_att_msg);
    string handleAttestationResult(RAMessages::AttestationMessage msg);
    void assembleMSG2(RAMessages::MessageMSG2 msg, sgx_ra_msg2_t **pp_msg2);
    string handleMSG2(RAMessages::MessageMSG2 msg);
    string handleMSG0(RAMessages::MessageMsg0 msg);
    string generateMSG1();
    
    string generateMSG0();
    string createInitMsg(int type, string msg);

protected:
    EnclaveManager *enclave = NULL;
#else 
private:
    string handleVerification();
#endif

private:
    int busy_retry_time = 4;
    NetworkManagerServer *nm = NULL;
};

#endif
