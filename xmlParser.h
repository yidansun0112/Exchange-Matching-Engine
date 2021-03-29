#ifndef XML_PARSER_H
#define XML_PARSER_H
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
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
using namespace xercesc;

class xmlParser{
private:
  std::string getChildValue(const char* parentTag, int parentIndex, const char* childTag, int childIndex);
  std::string getChildAttribute(const char* parentTag, int parentIndex, const char* childTag, int childIndex, const char* attributeTag);
  int getChildCount(const char* parentTag, int parentIndex, const char* childTag);
  std::vector<std::string> parseCreateXML();
  std::vector<std::string> parseTransactionsXML();
  //std::vector<std::string> parseResultsXML();
  std::string getParentAttribute(const char* parentTag, const char* attributeTag);
  DOMDocument* m_doc;
  void createParser();
 
public:
  std::vector<std::string> parseXML();
  std::string getRoot(); // here for test
  xmlParser(){};
  xmlParser(std::string inputXML);
  ~xmlParser(){
    if (m_doc) {
      m_doc->release();
    }
  }
};

class XmlDomErrorHandler : public HandlerBase
{
  public:
    void fatalError(const SAXParseException &exc) {
        printf("Fatal parsing error at line %d\n", (int)exc.getLineNumber());
        exit(1);
    }
};
#endif
