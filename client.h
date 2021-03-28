#include <iostream>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>


class Client{
private:
  const char* port_num;
  int server_fd;
  std::vector<std::string> requestXMLs;
  void sendRequest(int server_fd);
  void buildConnection();
public:
  Client():port_num("12345"){};
  void run();
};

