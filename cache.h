#ifndef CACHE_H
#define CACHE_H
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct CacheEntry {
  int value;
  chrono::steady_clock::time_point expiry;
};

class Cache {

private:
  unordered_map<int, CacheEntry> data;
  int time = 2;
  mutex mtx;
  int hashID(const string &id) { return static_cast<int>(hash<string>{}(id)); }

public:
  void insert(string account_no, int detail) {
    lock_guard<mutex> lock(mtx);
    int ID = hashID(account_no);
    data[ID] = {detail, chrono::steady_clock::now() + chrono::seconds(time)};
  }
  int search(string account_no) {
    lock_guard<mutex> lock(mtx);
    int ID = hashID(account_no);
    if (data.count(ID) == data.end())
      return -1;
    else {
      if (chrono::steady_clock::now() >= data[ID].expiry)
        data.erase(ID);
      else
        return data[ID].value;
    }
    return -1;
  }
};

#endif
