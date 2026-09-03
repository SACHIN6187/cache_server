#include "cache.h"

int Cache::hashID(const string &id) {
  return static_cast<int>(hash<string>{}(id));
}

void Cache::insert(string account_no, int detail) {
  int ID = hashID(account_no);
  data[ID] = new node(detail);
}

void Cache::update(string account_no, int Detail) {
  int ID = hashID(account_no);
  if (data.find(ID) == data.end())
    return;
  data[ID]->detail = Detail;
}

int Cache::search(string account_no) {
  int ID = hashID(account_no);

  if (data.find(ID) == data.end())
    return -1;

  return data[ID]->detail;
}