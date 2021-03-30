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


class xmlPrinter{
private:
  std::string getCreateXML(std::vector<std::string> tokens);
  std::string getTransactionXML(std::vector<std::string> tokens);
  std::string createHeader();
public:
  std::string createInvalidXML(std::string input);
  std::string createRequestXML(std::string input);
  std::string createCreateAccountXML(int userId, std::string msg);
  std::string createCreateSymbolXML(std::string symbol, int userId, std::string msg);
  std::string createOrderXML(std::string symbol, int amount, double limit, int transId, std::string msg);
  std::string createQueryXML(int transId,std::vector<std::string> msg);
  std::string createCancelXML(int transId, std::vector<std::string> msg);
  
};
