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
#include <fstream>
#include <vector>
#include "xmlParser.h"
#include "xmlPrinter.h"
#include "database.hpp"
class Server{
 private:
  const char* port_num;
  int listener;
  std::vector<std::string> requestXMLs;
  std::vector<char> responseXMLs;
  void setUpListener();
  static void * handleRequest(void * info);
  std::string createResponse();
  std::string executeTransactionsResult(std::vector<std::string> input, Database &db);
  std::string executeCreateResult(std::vector<std::string> input, Database &db);
  void createTables();
  std::string getXML(std::string msg);
public:
  Server() :port_num("12345"){}
  std::string executeParserResult(std::vector<std::string> input, Database &db);
  void sendString(int client_fd,string message);
  void run();
};

