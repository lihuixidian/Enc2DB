// //////////////////////////////////////////////////////////////////////////
// // Serializer.cpp
// #include "serializer.h"
// #include "LogBase.h"
// #include <fstream>

// using namespace util;


// Serializer::Serializer(void) : value_(Json::objectValue)
// {
// }

// Serializer::~Serializer(void)
// {
// }

// bool Serializer::FromFile(const std::string &sFile)
// {
//     if (sFile.empty())
//         return false;

//     // 获取文件大小
//     std::ifstream f(sFile.c_str());
//     f.seekg(std::ios::beg, std::ios::end);
//     std::streamsize file_size = f.tellg();
//     if (file_size <= 0)
//         return false;

//     // 从文件读取数据
//     f.seekg(0);
//     char *buffer = new char[file_size];
//     f.read(buffer, file_size);
//     f.close();

//     // 初始化
//     bool b = FromString(buffer);
//     delete buffer;
//     buffer = NULL;

//     return b;
// }

// bool Serializer::FromString(const std::string &sBuffer)
// {
//     Json::Reader reader;
//     if (!reader.parse(sBuffer, value_))
//     {
//         return false;
//     }

//     return true;
// }

// std::string Serializer::ToString()
// {
//     // Json::StreamWriterBuilder writer;
//     return value_.toStyledString();
// }

// bool Serializer::ToFile(const std::string &sSaveFile)
// {
//     if (sSaveFile.empty())
//         return false;

//     std::string sOut = ToString();

//     std::ofstream f(sSaveFile.c_str());
//     if (!f.good())
//     {
//         return false;
//     }

//     f.write(sOut.c_str(), sOut.size());
//     f.close();

//     return true;
// }