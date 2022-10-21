
#include "KeyManager.h"
#include "t.h"
#include "Singleton.h"

#include <elogDefination.h>


#ifdef ENCLAVE
    #include <sgx_error.h>
    #include <tlibc/mbusafecrt.h>
    #include <tlibc/errno.h>
    #include "ErrorSupport.h"
    #include "Enclave_t.h"
#else 
    #include <stdlib.h>
    #include <iostream>
    #include "LogBase.h"
    extern "C" {
        #include "postgres.h"
    }

#endif

using namespace std;

KeyManager::KeyManager()
    : _masterKey(new vector<uint8_t>(MasterKeySize)),
      _deriviedKeys(new vector<uint8_t>(TotalSize)),
      _status(NEW)
      {};

KeyManager::~KeyManager()
{
    saveAllKeysToFile();
}

void KeyManager::setPath(const string &mpath, const string &dpath)
{
    _masterKeyPath = mpath;
    _deriviedKeyPath = dpath;
    _status = ADDRESS;
}

void KeyManager::genMasterKey()
{
    uint8_t mkey[MasterKeySize];
    // _rand(mkey, MasterKeySize);
    //! TODO: for test
    for (int i = 0; i < MasterKeySize;i ++) {
        mkey[i] = 'l';
    }

    _masterKey->insert(_masterKey->begin(), mkey, mkey + sizeof(mkey));
    void *masterKeyFd = _open(_masterKeyPath.c_str(), "wb");
    assert(masterKeyFd);
    int size = _write(mkey, 1, sizeof(mkey), masterKeyFd);
    assert(size == sizeof(mkey));
    _close(masterKeyFd);
    // _status = LOADED;
}

void KeyManager::loadMasterKey()
{
    assert(_status == ADDRESS || _status == NEW);
    void *masterKeyFd = _open(_masterKeyPath.c_str(), "ab+");
    // assert(masterKeyFd);
    if(!masterKeyFd) {
        string errmsg = "can not open master key!, path=";
        errmsg += _masterKeyPath;
#ifdef ENCLAVE
        ocall_ereport(errmsg.c_str());
#else
        util::Log(errmsg, util::log::error);
#endif
        abort();
}

        
    int nsize = _read(_masterKey->data(), 1, MasterKeySize, masterKeyFd);
    _masterKey->resize(nsize);
    if (nsize != MasterKeySize)
        genMasterKey();
    _close(masterKeyFd);
    _status = LOADED;
}

void KeyManager::loadDeriviedKey()
{
    assert(_status == LOADED);
    void *deriviedKeyFd = _open(_deriviedKeyPath.c_str(), "ab+");
    assert(deriviedKeyFd);
    int nsize = _read(_deriviedKeys.get()->data(), 1, TotalSize, deriviedKeyFd);
    if (nsize != TotalSize)
        derivieKeys();
    _close(deriviedKeyFd);
    _status = DERIVIED;
}

void KeyManager::derivieKeys()
{
    if(_status < LOADED ) {
        loadMasterKey();
    }

    int curSize = 0;
    while (curSize < TotalSize)
    {
        vector<uint8_t> tmp(_masterKey->begin(), _masterKey->end());
        sha256Out_t out;
        _sha256((uint8_t *)(tmp.data()), tmp.size(), &out);
        for (int i = 0; i < sizeof(out); ++i) {
            _deriviedKeys->operator[](i + curSize) = out[i];
        }
            // _deriviedKeys->insert(_deriviedKeys->end(), out, ((uint8_t *)out) + 32);
        for (int i = 0; i < tmp.size(); i++)
            tmp[i] ^= _deriviedKeys->at(curSize + i);
        curSize += 32;
        // 
    }
    assert(curSize == _deriviedKeys->size());
    // _deriviedKeys->resize(TotalSize);
    // _status = DERIVIED;
}

void KeyManager::saveAllKeysToFile()
{
    if (_status >= LOADED)
    {
        int size;
        if (!_deriviedKeyPath.empty() && !_deriviedKeys->empty())
        {
            void *deriviedKeyFd = _open(_deriviedKeyPath.c_str(), "wb");
            assert(deriviedKeyFd);
            size = _write(_deriviedKeys->data(), 1, TotalSize, deriviedKeyFd);
            assert(size == TotalSize);
            _close(deriviedKeyFd);
        }

        if(!_masterKeyPath.empty() && !_masterKey->empty()) {
            void *masterKeyFd = _open(_masterKeyPath.c_str(), "wb");
            assert(masterKeyFd);
            size = _write(_masterKey->data(), 1, MasterKeySize, masterKeyFd);
            assert(size == MasterKeySize);
            _close(masterKeyFd);
        }

    }
}

void KeyManager::setMasterKey(const char* key) { //! length of key should be long enough
    int sz = strlen(key);
    assert(sz >= MasterKeySize);
    _masterKey->assign(key, key + MasterKeySize);
    _status = LOADED;
}

void KeyManager::saheKey(SaheKey_t k) 
{
    if (_status != DERIVIED)
    {
        derivieKeys();
        _status = DERIVIED;
    }
    mymemcpy(k, &(_deriviedKeys->at(SaheKeyIndex)), SaheKeySize);
}

void KeyManager::smheKey(SmheKey_t k) 
{
    if (_status != DERIVIED)
    {
        derivieKeys();
        _status = DERIVIED;
    }

    mymemcpy(k, &(_deriviedKeys->at(SmheKeyIndex)), SmheKeySize);
}

void KeyManager::aesKey(AesKey_t k) 
{
    if (_status != DERIVIED)
    {
        derivieKeys();
        _status = DERIVIED;
    }

    mymemcpy(k, &(_deriviedKeys->at(AesKeyIndex)), AesKeySize);
}
void KeyManager::oreKey(OreKey_t k) 
{
    if (_status != DERIVIED)
    {
        derivieKeys();
        _status = DERIVIED;
    }
    mymemcpy(k, &(_deriviedKeys->at(OreKeyIndex)), OreKeySize);
}

void KeyManager::masterKey(masterKey_t k) 
{
    assert(_status == LOADED);
    mymemcpy(k, _masterKey->data(), MasterKeySize);
}

#ifdef ENCLAVE
constexpr char MasterKeyPath[] = "/etc/encryptsql/masterKey.encrypted";
constexpr char DeriviedKeyPath[] = "/etc/encryptsql/deriviedKey.encrypted";
#else
constexpr char MasterKeyPath[] = "/etc/encryptsql/masterKey.plain";
constexpr char DeriviedKeyPath[] = "/etc/encryptsql/deriviedKey.plain";
#endif

string getAESKey()
{
    KeyManager::AesKey_t aes_keybuf;
    KeyManager *km = Singleton<KeyManager>::instance();
    if (km->status() == KeyManager::NEW)
    {
        km->setPath(MasterKeyPath, DeriviedKeyPath);
    }

    km->aesKey((uint8_t *)(aes_keybuf));
    return string(aes_keybuf, aes_keybuf + KeyManager::AesKeySize);
}

extern "C" char *getAESKey_c()
{
    auto aes_keybuf = (char*)malloc(32);
    string s = getAESKey();
    memcpy(aes_keybuf,s.c_str(), s.size());
    return aes_keybuf;
}

#ifdef ENCLAVE

#include <sgx_tprotected_fs.h>
#include <sgx_tcrypto.h>
#include <sgx_trts.h>

void *KeyManager::_open(const char *path, const char *mode)
{
    void *fp = sgx_fopen_auto_key(path, mode);
    return fp;
}
size_t KeyManager::_write(const void *src, size_t size, size_t count, void *stream)
{
    return sgx_fwrite(src, size, count, stream);
}
size_t KeyManager::_read(void *ptr, size_t size, size_t count, void *stream)
{
    return sgx_fread(ptr, size, count, stream);
}
int KeyManager::_close(void *stream)
{
    return sgx_fclose(stream);
}

void KeyManager::_rand(unsigned char *rand, size_t length_in_bytes)
{
    sgx_read_rand(rand, length_in_bytes);
}
void KeyManager::_sha256(const uint8_t *p_src, uint32_t src_len, sha256Out_t *p_hash)
{
    sgx_sha256_msg(p_src, src_len, p_hash);
}



#else

#include <unistd.h>
#include <time.h>
#include <math.h>
#include <memory.h>

#include <openssl/sha.h>

void *KeyManager::_open(const char *path, const char *mode)
{
    return fopen(path, mode);
}
size_t KeyManager::_write(const void *src, size_t size, size_t count, void *stream)
{
    return fwrite(src, size, count, (FILE *)stream);
}
size_t KeyManager::_read(void *ptr, size_t size, size_t count, void *stream)
{
    return fread(ptr, size, count, (FILE *)stream);
}
int KeyManager::_close(void *stream)
{
    return fclose((FILE *)stream);
}

void KeyManager::_rand(unsigned char *rand, size_t length_in_bytes)
{
    int cur = 0;
    while (cur < length_in_bytes)
    {
        srand(time(NULL)); //设置随机数种子，使每次产生的随机序列不同
        auto random = ::rand();
        int left = min(length_in_bytes - cur, sizeof(random));
        memcpy(rand + cur, (void *)&random, left);
        cur += left;
    }
}
void KeyManager::_sha256(const uint8_t *p_src, uint32_t src_len, sha256Out_t *p_hash)
{
    SHA256(p_src, src_len, *p_hash);
}

#endif