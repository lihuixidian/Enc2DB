
#pragma once

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdarg.h>
#include <string>



#include <memory>
#include <vector>

using namespace std;


class KeyManager
{

public:
    static constexpr char MasterKeySize = 32;

    static constexpr char SaheKeyIndex = 0;
    static constexpr char SaheKeySize = 32;

    static constexpr char SmheKeyIndex = SaheKeyIndex + SaheKeySize;
    static constexpr char SmheKeySize = 32;

    static constexpr char AesKeyIndex = SmheKeyIndex + SmheKeySize;
    static constexpr char AesKeySize = 32;

    static constexpr char OreKeyIndex = AesKeyIndex + AesKeySize;
    static constexpr char OreKeySize = 32;

    static constexpr int TotalSize = 128;

    typedef uint8_t SaheKey_t[SaheKeySize];
    typedef uint8_t SmheKey_t[SmheKeySize];
    typedef uint8_t AesKey_t[SaheKeySize];
    typedef uint8_t OreKey_t[OreKeySize];
    typedef uint8_t sha256Out_t[32];

    enum
    {
        NEW,
        ADDRESS,
        LOADED,   // MasterKey is loaded
        DERIVIED, // deriviedKey got
    };
    

private:
    unique_ptr<vector<uint8_t>> _masterKey;
    unique_ptr<vector<uint8_t>> _deriviedKeys;

    string _masterKeyPath = "/etc/encryptsql/masterKey.encrypted";
    string _deriviedKeyPath = "/etc/encryptsql/deriviedKey.encrypted";
    int _status = NEW;

    // 以下函数由调用库的一方实现
    void* _open(const char* path, const char* mode) ;
    size_t _write(const void *src, size_t size, size_t count, void *stream);
    size_t _read(void *ptr, size_t size, size_t count, void *stream);
    int _close(void *stream);

    void _rand(unsigned char *rand, size_t length_in_bytes);
    void _sha256(const uint8_t *p_src, uint32_t src_len, sha256Out_t *p_hash);

public:
    KeyManager();
    ~KeyManager();
    void setPath(const string &mpath, const string &dpath);
    void genMasterKey();
    void loadMasterKey();
    void loadDeriviedKey();
    void derivieKeys();
    void saveAllKeysToFile();

    void saheKey(SaheKey_t k);
    void smheKey(SmheKey_t k);
    void aesKey(AesKey_t k);
    void oreKey(OreKey_t k);
    int status() const { return _status; }
};
