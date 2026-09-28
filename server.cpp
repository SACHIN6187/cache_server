#include "cache.h"
// g++ -std=c++17 server.cpp cache.cpp -o cache_server
//./cache_server

#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main() {

  Cache myCache;

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (serverSocket < 0) {
    cerr << "Socket creation failed\n";
    return 1;
  }

  sockaddr_in serverAddress{};

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  int port = 6000;
  if (getenv("PORT")) {
    port = stoi(getenv("PORT"));
  }

  serverAddress.sin_port = htons(port);

  if (::bind(serverSocket, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) < 0) {

    cerr << "Bind failed\n";
    close(serverSocket);
    return 1;
  }

  if (listen(serverSocket, 5) < 0) {

    cerr << "Listen failed\n";
    close(serverSocket);
    return 1;
  }

  cout << "Cache server running on port " << port << "...\n";

  while (true) {

    cout << "Waiting for connection...\n";

    int clientSocket = accept(serverSocket, nullptr, nullptr);

    if (clientSocket < 0) {
      cerr << "Accept failed\n";
      continue;
    }
    cout << "Client connected!\n";
    char buffer[1024] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
      string request(buffer, bytesReceived);
      cout << "Received: " << request << endl;
      if (request.substr(0, 4) == "GET ") {
        string accountNo = request.substr(4);
        if (!accountNo.empty() &&
            (accountNo.back() == '\n' || accountNo.back() == '\r')) {
          accountNo.pop_back();
        }
        int balance = myCache.search(accountNo);
        string response = to_string(balance);
        send(clientSocket, response.c_str(), response.size(), 0);
      } else if (request.substr(0, 4) == "SET ") {
        string ammount, accountNo;
        int n = request.size();
        bool acc_go = true;
        for (int i = 4; i < n; i++) {
          if (request[i] == ' ') {
            acc_go = false;
            continue;
          }
          if (acc_go)
            accountNo += request[i];
          else
            ammount += request[i];
        }

        myCache.insert(accountNo, stoll(ammount));
      }
    }

    close(clientSocket);
  }

  close(serverSocket);

  return 0;
}
