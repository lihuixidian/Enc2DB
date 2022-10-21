//////////////////////////////////////////////////////////////////////////
// Config.cpp
#include "config.h"
#include "LogBase.h"
#include <fstream>

using namespace util;


Config::Config(void) : value_(Json::objectValue)
{
    if (!FromFile(CONFIG_PATH))
    {
        // Log("cannot open config path:%s\n", CONFIG_PATH, log::error); // log 也需要Config，这样就死循环了
        throw std::exception();
    }
}

Config::~Config(void)
{
}

bool Config::FromFile(const std::string &sFile)
{
    if (sFile.empty())
        return false;

    // 获取文件大小
    std::ifstream f(sFile.c_str());
    f.seekg(std::ios::beg, std::ios::end);
    std::streamsize file_size = f.tellg();
    if (file_size <= 0)
        return false;

    // 从文件读取数据
    f.seekg(0);
    char *buffer = new char[file_size];
    f.read(buffer, file_size);
    f.close();

    // 初始化
    bool b = FromString(buffer);
    delete buffer;
    buffer = NULL;

    return b;
}

bool Config::FromString(const std::string &sBuffer)
{
    Json::Reader reader;
    if (!reader.parse(sBuffer, value_))
    {
        return false;
    }

    return true;
}

std::string Config::ToString()
{
    // Json::StreamWriterBuilder writer;
    // return writer.write(value_);
    return value_.toStyledString();
}

bool Config::ToFile(const std::string &sSaveFile)
{
    if (sSaveFile.empty())
        return false;

    std::string sOut = ToString();

    std::ofstream f(sSaveFile.c_str());
    if (!f.good())
    {
        return false;
    }

    f.write(sOut.c_str(), sOut.size());
    f.close();

    return true;
}

const char *Config::getFloatScaleCiphertext()
{
    if(scaleCiphertext.size()) {
        // TODO: 使用远程认证来填写scale的密文
    }
    return scaleCiphertext.c_str();
}

void Config::setFloatScaleCiphertext(const std::string& s)
{
    scaleCiphertext = s;
}

extern "C" const char *getFloatScaleCiphertext()
{
    auto config = Config::getInstance();
    config->setFloatScaleCiphertext(config->Get("user.scaleSMHECipher","null").asString() ); // TODO；改为远程认证填充cipher
    return Config::getInstance()->getFloatScaleCiphertext();
}

#ifdef USING_TestCode
class TestCode_Config
{
public:
    TestCode_Config()
    {
        Config config;
        config.FromString("{}");
        config.Set("test.base.savepath", "D:/test");

        Json::Value v = config.Get("base.savepath", "");
        std::string s = v.asString();
        OutputDebugStringA(s.c_str());
        std::string sJsonString;
        sJsonString = config.ToString();
        OutputDebugStringA(sJsonString.c_str());
    }

    ~TestCode_Config()
    {
    }
} TestCode_Config_c;
#endif