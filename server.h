#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>

class Server{
 private:
  const char* port_num;
  int listener;
  std::vector<std::string> requestXMLs;
  std::vector<char> responseXMLs;
  void setUpListener();
  void handleRequest(int client_fd);
public:
  Server() :port_num("12345"){};
  void run();
};

