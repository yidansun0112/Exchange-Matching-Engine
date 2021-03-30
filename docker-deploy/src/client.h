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
#include <string>
#include "xmlPrinter.h"

using namespace std;


class Client{
private:
  const char* hostname;
  const char* port_num;
  const char * filename;
  int server_fd;
  std::vector<std::string> requestXMLs;
  void sendRequest();
  void recvResponse();
public:
  Client(const char * host, const char * name):hostname(host),port_num("12345"),filename(name){};
  void run();
  void buildConnection();
  void sendRequest(string str);
  void sendString(string message);
  void closefd(){
    close(server_fd);
  }
};

class Thread_info{
public:
    const char * hostname;
    const char * filename;
    Thread_info(const char * hostname, const char * filename):hostname(hostname),filename(filename){}
};