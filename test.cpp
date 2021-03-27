#include <cstdio>
#include "xmlPrinter.h"
#include "xmlParser.h"
int main() {
  xmlPrinter printer;
  std::ifstream newfile ("xml.txt");
  if (newfile.is_open()){   //checking whether the file is open
    std::string tp;
    while(getline(newfile, tp)){
      std::cout<<printer.createRequestXML(tp);
      xmlParser* parser = new xmlParser(printer.createRequestXML(tp));
      std::vector<std::string> result = parser->parseXML();
      for (size_t i = 0; i < result.size(); i++) {
        std::cout << result[i] << std::endl;
      }
    }
    newfile.close(); //close the file object.
  }
  return 0;
}
