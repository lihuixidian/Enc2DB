

#include <memory>

#include "Singleton.h"
#include "statusReport.h"
#include "routineDecider.h"
#include "Timer.h"
#include "errmsgBridge.h"
#include "TimeCounter.h"
#include "LogBase.h"
extern "C"
{
#include "Interface.h"
#include "udfDispatch.h"
}
using namespace util;
#ifdef USE_SGX
#include "Enclave_u.h"
#endif
#include "App.h"

#ifndef NDEBUG  // DEBUG
    #include <fstream>
    extern "C"
    {
    #include <unistd.h>
    }
    using namespace std;

    class CalledCounter
    {
    public:
        CalledCounter()
        {
            char buf[64];
            sprintf(buf, "/etc/encryptsql/UDFcalled_by%d.count", (int)getpid());

            ofstream fout(buf);
            fout.close();
        }
    };
    CalledCounter c;
#endif

extern "C" int ore_compare(char *strctxt1, char *strctxt2);

constexpr char TERMCHAR = '^';

char *saheAdd(char *cipher1, char *cipher2, char *aescipher1, char *aescipher2)
{
    size_t csz1 = strlen(cipher1) + 1; // csz1 out of range, csz1 - 1 should be \0, csz1 - 2 should be TERM
    size_t csz2 = strlen(cipher2) + 1;
    if (!(cipher1[csz1 - 2] == TERMCHAR && cipher2[csz2 - 2] == TERMCHAR))
    {
        Log("saheAdd: cannot find term char in cipher1(%s) or cipher2(%s)!", cipher1, cipher2, log::error);
        abort();
    }
    
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unique_ptr<StatusReport> rep(
        new StatusReport(SAHE_ADD, csz1 + csz2));

    EnclaveManager *enclave = EnclaveManager::getInstance();
    auto decider = Singleton<RoutineDecider>::instance();

    ExecutionPath path = decider->makeDecision(rep.get());

    char *ret;

    Timer t = Timer();
    if (path == HARDWARE && ecall_sahe_add)
    {
        if (strcmp(aescipher1, "null") == 0)
           Log("client does not support SGX while Server try to calculate AHE with SGX!\n", log::error);
        ret = new char[csz1 + csz2];
        
        ecall_sahe_add(enclave->getID(), aescipher1, aescipher2, ret, csz1 + csz2);
    }
    else
    {
        path = SOFTWARE;
        ret = SAHE_add_udf(cipher1, cipher2, csz1, csz2);
    }

    rep->_runningTime = t.passedTimeMicroSecond();
    rep->_path = path;

    decider->report(rep);
    int retsz = strlen(ret);
    if (retsz < 16 || ret[retsz - 1] != TERMCHAR) {
        Log("saheAdd: cannot find term char in returned buffer(%s)", ret, log::error);
        abort();
    }
       
    return ret;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

char *saheSub(char *cipher1, char *cipher2, char *aescipher1, char *aescipher2)
{

    size_t csz1 = strlen(cipher1) + 1;
    size_t csz2 = strlen(cipher2) + 1;
    if (!(cipher1[csz1 - 2] == TERMCHAR && cipher2[csz2 - 2] == TERMCHAR))
    {
        Log("saheSub: cannot find term char in cipher1(%s) or cipher2(%s)!", cipher1, cipher2, log::error);
        abort();
    }
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unique_ptr<StatusReport> rep(
        new StatusReport(SAHE_SUB, csz1 + csz2));

    auto decider = Singleton<RoutineDecider>::instance();
    ExecutionPath path = decider->makeDecision(rep.get());

    char *ret = nullptr;

    Timer t = Timer();
    if (path == HARDWARE && ecall_sahe_sub)
    {
        EnclaveManager *enclave = EnclaveManager::getInstance();
        ret = new char[csz1];
        if (strcmp(aescipher1, "null") == 0)
           Log("client does not support SGX while Server try to calculate AHE_SUB with SGX!\n", log::error);
        ecall_sahe_sub(enclave->getID(), aescipher1, aescipher2, ret, csz1);
    }
    else
        ret = SAHE_substract_udf(cipher1, cipher2, csz1, csz2);
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    rep->_path = path;
    rep->_runningTime = t.passedTimeMicroSecond();

    decider->report(rep);
    return ret;
}

char *saheMul(char *cipher1, int m)
{
    size_t csz1 = strlen(cipher1) + 1;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unique_ptr<StatusReport> rep(
        new StatusReport(SAHE_MUL, csz1));

    auto decider = Singleton<RoutineDecider>::instance();
    ExecutionPath path = decider->makeDecision(rep.get());

    if (strcmp(cipher1, "null") == 0)
    {
#if !defined(USE_SGX) && !defined(SGX_ORE) // 前两个正常ORE为空，说明客户端有SGX_ORE，此时服务器没有SGX_ORE的话无法解密。
       Log("client use SGX to calculate MHE but server does not support SGX!\n", log::error);
#endif
    }

    char *ret = nullptr;
    Timer t = Timer();
    if (path == HARDWARE && ecall_sahe_mul)
    {
        EnclaveManager *enclave = EnclaveManager::getInstance();
        ret = new char[csz1];

        ecall_sahe_mul(enclave->getID(), cipher1, m, ret, csz1);
    }
    else
        ret = SAHE_multiply_udf(cipher1, m, csz1);
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    rep->_path = path;
    rep->_runningTime = t.passedTimeMicroSecond();
    decider->report(rep);
    return ret;
}

char *saheNeg(char *cipher1)
{
    size_t csz1 = strlen(cipher1) + 1;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unique_ptr<StatusReport> rep(
        new StatusReport(SAHE_NEG, csz1));

    auto decider = Singleton<RoutineDecider>::instance();
    ExecutionPath path = decider->makeDecision(rep.get());

    char *ret = nullptr;
    Timer t = Timer();
    if (path == HARDWARE && ecall_sahe_neg)
    {
        EnclaveManager *enclave = EnclaveManager::getInstance();
        ret = new char[csz1];

        ecall_sahe_neg(enclave->getID(), cipher1, ret, csz1);
    }
    else
        ret = SAHE_neggate_udf(cipher1, csz1);
    rep->_path = path;
    rep->_runningTime = t.passedTimeMicroSecond();
    decider->report(rep);
    return ret;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

char *smheMul(char *cipher1, char *cipher2, char *aescipher1, char *aescipher2)
{
    size_t csz1 = strlen(cipher1) + 1;
    size_t csz2 = strlen(cipher2) + 1;
    if (!(cipher1[csz1 - 2] == TERMCHAR && cipher2[csz2 - 2] == TERMCHAR))
    {
        Log("smheMul: cannot find term char in cipher1(%s) or cipher2(%s)!", cipher1, cipher2, log::error);
        abort();
    }
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unique_ptr<StatusReport> rep(
        new StatusReport(SMHE_MUL, csz1 + csz2));

    auto decider = Singleton<RoutineDecider>::instance();
    ExecutionPath path = decider->makeDecision(rep.get());

#ifdef SGX_MHE
    path = HARDWARE;
#endif
    char *ret = nullptr;
    Timer t = Timer();
    if (path == HARDWARE && ecall_smhe_mul)
    {
        EnclaveManager *enclave = EnclaveManager::getInstance();
        ret = new char[csz1];
        if (strcmp(aescipher1, "null") == 0)
           Log("client does not support SGX while Server try to calculate MHE with SGX!\n", log::error);
        ecall_smhe_mul(enclave->getID(), aescipher1, aescipher2, ret, csz1);
    }
    else
    {
        // char *__ = new char[csz1];
        ret = SMHE_MULTIPLY_udf(cipher1, cipher2, csz1, csz2);
    }

    rep->_path = path;
    rep->_runningTime = t.passedTimeMicroSecond();
    decider->report(rep);
    return ret;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

char *smheMulp(char *cipher1, int m)
{

    size_t csz1 = strlen(cipher1) + 1;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unique_ptr<StatusReport> rep(
        new StatusReport(SMHE_MUL, csz1));

    auto decider = Singleton<RoutineDecider>::instance();
    ExecutionPath path = decider->makeDecision(rep.get());
    char *ret = nullptr;
    Timer t = Timer();
    if (path == HARDWARE && ecall_smhe_mulp)
    {
        EnclaveManager *enclave = EnclaveManager::getInstance();
        ret = new char[csz1];

        ecall_smhe_mulp(enclave->getID(), cipher1, m, ret, csz1);
    }
    else
        ret = SMHE_MULTIPLYPLAIN_udf(cipher1, m, csz1);
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    rep->_path = path;
    rep->_runningTime = t.passedTimeMicroSecond();
    decider->report(rep);
    return ret;
}

char *smheDiv(char *cipher1, char *cipher2, char *aescipher1, char *aescipher2)
{
    size_t csz1 = strlen(cipher1) + 1;
    size_t csz2 = strlen(cipher2) + 1;
    if (!(cipher1[csz1 - 2] == TERMCHAR && cipher2[csz2 - 2] == TERMCHAR))
    {
        Log("smheDiv: cannot find term char in cipher1(%s) or cipher2(%s)!", cipher1, cipher2, log::error);
        abort();
    }
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unique_ptr<StatusReport> rep(
        new StatusReport(SMHE_DIV, csz1 + csz2));

    auto decider = Singleton<RoutineDecider>::instance();
    ExecutionPath path = decider->makeDecision(rep.get());
    char *ret = nullptr;
    Timer t = Timer();
    if (path == HARDWARE && ecall_smhe_div)
    {
        EnclaveManager *enclave = EnclaveManager::getInstance();
        ret = new char[csz1];
        if (strcmp(aescipher1, "null") == 0)
           Log("client does not support SGX while Server try to calculate MHE with SGX!\n", log::error);
        ecall_smhe_div(enclave->getID(), aescipher1, aescipher2, ret, csz1);
    }
    else
        ret = SMHE_DIVIDE_udf(cipher1, cipher2, csz1, csz2);
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    rep->_path = path;
    rep->_runningTime = t.passedTimeMicroSecond();
    decider->report(rep);
    return ret;
}

char *smhePow(char *cipher1, char *aescipher1, int m)
{
    size_t csz1 = strlen(cipher1) + 1;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unique_ptr<StatusReport> rep(
        new StatusReport(SMHE_POW, csz1));

    auto decider = Singleton<RoutineDecider>::instance();
    ExecutionPath path = decider->makeDecision(rep.get());
    char *ret = nullptr;
    Timer t = Timer();
    if (path == HARDWARE && ecall_smhe_pow)
    {
        EnclaveManager *enclave = EnclaveManager::getInstance();
        ret = new char[csz1 * 2];
        ecall_smhe_pow(enclave->getID(), aescipher1, m, ret, csz1);
    }
    else
        ret = SMHE_POW_udf(cipher1, m, csz1);
    rep->_path = path;
    rep->_runningTime = t.passedTimeMicroSecond();
    decider->report(rep);
    return ret;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

char *smheInv(char *cipher1)
{

    size_t csz1 = strlen(cipher1) + 1;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unique_ptr<StatusReport> rep(
        new StatusReport(SMHE_INV, csz1));

    auto decider = Singleton<RoutineDecider>::instance();
    ExecutionPath path = decider->makeDecision(rep.get());
    char *ret = nullptr;
    Timer t = Timer();
    if (path == HARDWARE && ecall_smhe_inv)
    {
        EnclaveManager *enclave = EnclaveManager::getInstance();
        char *ret = new char[csz1];
        ecall_smhe_inv(enclave->getID(), cipher1, ret, csz1);
    }
    else
        ret = SMHE_INVERSE_udf(cipher1, csz1);
    rep->_path = path;
    rep->_runningTime = t.passedTimeMicroSecond();
    decider->report(rep);
    return ret;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

char *saheAddp(char *cipher1, int m)
{
    size_t sz1 = strlen(cipher1);
    return SAHE_addplaintext_udf(cipher1, m, sz1);
}

string getOREType(int kind)
{
    switch (kind)
    {
    case 1:
        return "LT";
    case 2:
        return "LE";
    case 3:
        return "GT";
    case 4:
        return "GE";
    case 5:
        return "EE";
    case 6:
        return "MAX";
    case 7:
        return "MIN";
        return "UN";
    }
}

int oreCompare(char *cipher1, char *cipher2, char *aescipher1, char *aescipher2, int kind)
{
    size_t csz1 = strlen(cipher1) + 1;
    size_t csz2 = strlen(cipher2) + 1;

    if (strcmp(cipher1, "null") == 0)
    {
#if !defined(USE_SGX) && !defined(SGX_ORE) // 前两个正常ORE为空，说明客户端有SGX_ORE，此时服务器没有SGX_ORE的话无法解密。
       Log("client use SGX to calculate ORE but server does not support SGX!\n", log::error);
#endif
    }

    //!ORE
    int res = -2;
    unique_ptr<StatusReport> rep(
        new StatusReport(ORE, csz1));

    auto decider = Singleton<RoutineDecider>::instance();

    ExecutionPath path = decider->makeDecision(rep.get());
#ifdef SGX_ORE
    path = HARDWARE;
#endif

    int ret = -3;
    Timer t = Timer();
    if (path == HARDWARE && ecall_ore_compare)
    {
        if (strcmp(aescipher1, "null") == 0)
           Log("client does not support SGX while Server try to calculate ORE with SGX!\n", log::error);

        EnclaveManager *enclave = EnclaveManager::getInstance();
        ecall_ore_compare(enclave->getID(), aescipher1, aescipher2, &ret);
    }
    else
    {
        path = SOFTWARE;
        ret = ore_compare(cipher1, cipher2);
    }

    rep->_path = path;
    rep->_runningTime = t.passedTimeMicroSecond();
    // TimeCounter *counter = Singleton<TimeCounter>::instance();
    // counter->count(getOREType(kind), t.passedTimeMicroSecond());
    decider->report(rep);
    return ret;
}