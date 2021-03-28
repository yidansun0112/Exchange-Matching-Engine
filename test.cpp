#include <cstdio>
#include "xmlParser.h"
#include <pqxx/pqxx>
#include "server.h"
using namespace pqxx;

int main() {
  //open database
  Server* testServer = new Server();
  xmlPrinter printer;
  std::ifstream newfile ("xml.txt");
  if (newfile.is_open()){   //checking whether the file is open
    std::string tp;
    while(getline(newfile, tp)){
      std::cout<<printer.createRequestXML(tp);
      xmlParser* parser = new xmlParser(printer.createRequestXML(tp));
      std::vector<std::string> result = parser->parseXML();
      testServer->executeParserResult(result);
    }
    newfile.close(); //close the file object.
  }
  return 0;
}
