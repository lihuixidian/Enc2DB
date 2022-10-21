
#pragma once
#include "Timer.h"

#include <unordered_map>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>

// ! use json
// #define USE_JSON

#ifdef USE_JSON
#include <json/json.h>
#endif
using namespace std;
class TimeCounter
{

    struct Node
    {
        double avgTime = 0;
        double cntTime = 0;
        double maxTime = 0;
        size_t callCnt = 0;
    };

private:
    unordered_map<string, Node> _counter;
#ifdef USE_JSON
    Json::Value root;
    string jsonPath = "/etc/encryptsql/counter.json";
#endif
public:
    bool exists(const std::string &name)
    {
        ifstream f(name.c_str());
        if (f.good())
        {
            f.close();
            return true;
        }
        return false;
    }

    void count(const string &str, double time)
    {
        if (_counter.count(str) == 0)
        {
            // _counter[str].avgTime = time;
            _counter[str].cntTime = time;
            _counter[str].maxTime = time;
            _counter[str].callCnt = 0;
        }
        else
        {
            // _counter[str].avgTime = 0.5 * (time) + 0.5 * _counter[str].avgTime;
            _counter[str].maxTime = max(_counter[str].maxTime, time);
            _counter[str].cntTime += time;
        }

        ++_counter[str].callCnt;
    }
#ifdef USE_JSON

    Json::Value getJsonNode()
    {
        Json::Value node;
        node["avgTime"] = 0.0;
        node["cntTime"] = 0.0;
        node["maxTime"] = 0.0;
        node["callCnt"] = 0l;
        return node;
    }
    void addToNode(Node &node, Json::Value &v)
    {
        node.cntTime += v["cntTime"].asDouble();
        node.callCnt += v["callCnt"].asUInt64();
        node.maxTime = max(node.maxTime, v["maxTime"].asDouble());
    }

#endif
    TimeCounter()
    {
#ifdef USE_JSON
        if (!exists(jsonPath))
            return;

        std::fstream fin(jsonPath, std::fstream::in);
        if (fin.is_open())
        {
            Json::CharReaderBuilder builder;
            builder["collectComments"] = true;
            JSONCPP_STRING errs;
            if (!fin.eof() && !parseFromStream(builder, fin, &root, &errs))
                std::cout << errs << std::endl;
        }
        fin.close();

        for (auto it = root["data"].begin(); it != root["data"].end(); it++)
        {
            string key = it.key().asString();
            if ((*it)[key].isNull())
                continue;
            addToNode(_counter[key], (*it)[key]);
        }

#endif
    }
    ~TimeCounter()
    {
        int pid = getpid();
#ifdef USE_JSON
        if (root["cnt"].isNull())
            root["cnt"] = 1;
        pid = root["cnt"].asInt();

#endif
        char path1[64] = {0};
        char path2[64] = {0};
        sprintf(path1, "/etc/encryptsql/counter_%d.log", pid);
        sprintf(path2, "/etc/encryptsql/counter_%d.json", pid);
        FILE *fp = fopen(path1, "w");
        FILE *jsonFp = fopen(path2, "w");
        if (!fp || !jsonFp)
        {
            cout << "Can not open counter file" << endl;
            exit(-1);
        }

        for (auto &kv : _counter)
        {
            kv.second.avgTime = ((double)kv.second.cntTime / (double)kv.second.callCnt);
            fprintf(fp, "%s: \nAvgTime=%.2lfus, maxTime=%.2lfus, CntTime=%.4lfs, Called=%ld\n\n", kv.first.c_str(), kv.second.avgTime, kv.second.maxTime,
                    kv.second.cntTime / Timestamp::kMicroSecondsPerSecond, kv.second.callCnt);

            fprintf(jsonFp, "{name:\"%s\", value : %.2lf },\n", kv.first.c_str(), kv.second.avgTime);
        }
        fclose(fp);
        fclose(jsonFp);

#ifdef USE_JSON

        for (auto &kv : _counter)
        {
            auto node = getJsonNode();
            node["cntTime"] = kv.second.cntTime;
            node["callCnt"] = kv.second.callCnt;
            node["maxTime"] = kv.second.maxTime;
            Json::Value v;
            v[kv.first] = node;
            root["data"].append(v);
        }
        root["cnt"] = pid + 1;

        std::ofstream fout(jsonPath);
        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(root, &fout);
        fout << std::endl; // add lf and flush
        fout.close();

#endif
    }
};

extern "C" void c_count(const char* name, double time);
