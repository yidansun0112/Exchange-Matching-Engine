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
  std::string createRequestXML(std::string input);
  std::string createCreateResponseXML(std::string);
  std::string createTransactionResponseXML(std::string);
};
