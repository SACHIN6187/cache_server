#ifndef CACHE_H
#define CACHE_H

#include <functional>
#include <iostream>
#include <map>
#include <string>

using namespace std;

class Cache {
private:
  struct node {
    int detail;

    node(int detail) : detail(detail) {}
  };

  map<int, node *> data;

  int hashID(const string &id);

public:
  void insert(string account_no, int detail);
  void update(string account_no, int detail);
  int search(string account_no);
};

#endif