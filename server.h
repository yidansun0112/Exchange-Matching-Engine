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
  Database db;
  int listener;
  std::vector<std::string> requestXMLs;
  std::vector<char> responseXMLs;
  void setUpListener();
  void handleRequest(int client_fd);
  std::string createResponse();
  void executeTransactionsResult(std::vector<std::string> input);
  void executeCreateResult(std::vector<std::string> input);
public:
  Server() :port_num("12345"){
    try{
      db.openDatabase();
    }catch (const exception &e){
      return;
    }
    db.createTables();
    db.disconnect();
  };
  void executeParserResult(std::vector<std::string> input);
 
  void run();
};

