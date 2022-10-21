
#ifdef USE_LRU

#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

#define DEFAULT_LRU_CAPACITY  16 * 1024  // 16K 个数据

template <typename K, typename V>
struct DLinkedNode
{
    K key;
    V value;
    DLinkedNode<K,V> *prev;
    DLinkedNode<K,V> *next;
    DLinkedNode() : key(), value(), prev(nullptr), next(nullptr) {}
    DLinkedNode(const K& _key, V _val) : key(_key), value(_val), prev(nullptr), next(nullptr) {}
};

template <typename K, typename V>
class LRUCache
{
private:
    std::unordered_map<K, std::unique_ptr<DLinkedNode<K, V>>> cache;
    std::unique_ptr<DLinkedNode<K, V>> head;
    std::unique_ptr<DLinkedNode<K, V>> tail;

    size_t size = 0;
    size_t _capacity;
    string _name;   // 析构时需要将结果写入到文件中，将此name作为文件名

    void addToHead(DLinkedNode<K, V> *node);
    void removeNode(DLinkedNode<K, V> *node);
    void moveToHead(DLinkedNode<K, V> *node);
    void removeTail();

public:
    long long requestTimes = 0;
    long long hitTimes = 0;
    long long swapOutTimes = 0;
    long long putTimes = 0;
    ~LRUCache();
    LRUCache(size_t _capacity = DEFAULT_LRU_CAPACITY, string name = "Anonymous");

    size_t capacity() const {
        return _capacity;
    }
    double hitRatio() const {
        if(requestTimes == 0)
            return -2;
        return (double)hitTimes / (double)requestTimes;
    }

    double swapOutRatio() const {
        if(requestTimes == 0)
            return -2;
        return (double)swapOutTimes / (double)putTimes;
    }

    void setName(const string& name) {
        _name = name;
    }
    V get(const K &key, const V &defaultv);

    void put(const K &key, const V &value);
};




template <typename K, typename V>
LRUCache<K,V>::LRUCache(size_t _capacity, string name)
    : head(new DLinkedNode<K, V>()), tail(new DLinkedNode<K, V>()), _capacity(_capacity), _name(name)
{
    head->next = tail.get();
    tail->prev = head.get();
}

template <typename K, typename V>
V LRUCache<K,V>::get(const K &key, const V& defaultv)
{
    ++requestTimes;
    if (!cache.count(key))
    {
        return defaultv;
    }
    ++hitTimes;
    DLinkedNode<K, V> *node = cache[key].get();
    moveToHead(node);
    return node->value;
}

template <typename K, typename V>
void LRUCache<K,V>::put(const K &key, const V &value)
{
    putTimes++;
    if (!cache.count(key))
    {
        DLinkedNode<K, V> *node = new DLinkedNode<K, V>(key, value);
        cache[key] = std::unique_ptr<DLinkedNode<K, V>>(node);
        addToHead(node);
        ++size;
        while (size > _capacity)
        {
            ++swapOutTimes;
            removeTail();
            --size;
        }
    }
    else
    {
        DLinkedNode<K, V> *node = cache[key].get();
        node->value = value;
        moveToHead(node);
    }
}

template <typename K, typename V>
void LRUCache<K,V>::addToHead(DLinkedNode<K, V> *node)
{
    node->prev = head.get();
    node->next = head->next;
    head->next->prev = node;
    head->next = node;
}
template <typename K, typename V>
void LRUCache<K,V>::removeNode(DLinkedNode<K, V> *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}
template <typename K, typename V>
void LRUCache<K,V>::moveToHead(DLinkedNode<K, V> *node)
{
    removeNode(node);
    addToHead(node);
}
template <typename K, typename V>
void LRUCache<K,V>::removeTail()
{
    DLinkedNode<K, V> *node = tail->prev;
    removeNode(node);
    cache.erase(node->key);
    // return node;
}
template <typename K, typename V>
LRUCache<K, V>::~LRUCache()
{
#if defined(TIME_PORTRAIT) && !defined(ENCLAVE)
    char buf[64] = {0};
    sprintf(buf, "/etc/encryptsql/ClientLRUreport_%s.log", _name.c_str());
    FILE *fp = fopen(buf, "w");
    if (fp == NULL)
    {
        printf("cannot open lru report file to write\n");
        abort();
    }
    double hit = (double)hitTimes / (double)requestTimes;
    fprintf(fp, "Cache 命中率: %.2lf%%\n", hitRatio() * 100);
    fprintf(fp, "Cache 置换率: %.2lf%%\n", swapOutRatio() * 100);
    fclose(fp);
#endif
}


#endif