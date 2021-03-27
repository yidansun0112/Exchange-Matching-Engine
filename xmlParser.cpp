#include "xmlParser.h"
using namespace xercesc;

XercesDOMParser* parser = NULL;
ErrorHandler* errorHandler = NULL;


xmlParser::xmlParser(std::string inputXML):m_doc(NULL) {
  createParser();
  xercesc::MemBufInputSource src((const XMLByte*)inputXML.c_str(), inputXML.length(), "dummy", false);
  parser->parse(src);
  m_doc = parser->adoptDocument();
}
 

void xmlParser::createParser() {
  if (!parser) {
    XMLPlatformUtils::Initialize();
    parser = new XercesDOMParser();
    errorHandler = (ErrorHandler*) new XmlDomErrorHandler();
    parser->setErrorHandler(errorHandler);
  }
}

std::string xmlParser::getChildValue(const char* parentTag, int parentIndex, const char* childTag, int childIndex) {
  XMLCh* temp = XMLString::transcode(parentTag);
  DOMNodeList* list = m_doc->getElementsByTagName(temp);
  XMLString::release(&temp);
  DOMElement* parent = dynamic_cast<DOMElement*>(list->item(parentIndex));
  DOMElement* child = dynamic_cast<DOMElement*>(parent->getElementsByTagName(XMLString::transcode(childTag))->item(childIndex));
  std::string ans;
  if (child) {
    char* temp2 = XMLString::transcode(child->getTextContent());
    ans = temp2;
    XMLString::release(&temp2);
  }
  else {
    ans = "";
  }
  return ans;
}

std::string xmlParser::getChildAttribute(const char* parentTag, int parentIndex, const char* childTag, int childIndex, const char* attributeTag) {
  XMLCh* temp = XMLString::transcode(parentTag);
  DOMNodeList* list = m_doc->getElementsByTagName(temp);
  XMLString::release(&temp);
  DOMElement* parent = dynamic_cast<DOMElement*>(list->item(parentIndex));
  DOMElement* child = dynamic_cast<DOMElement*>(parent->getElementsByTagName(XMLString::transcode(childTag))->item(childIndex));
  std::string ans;
  if (child) {
    temp = XMLString::transcode(attributeTag);
    char* temp2 = XMLString::transcode(child->getAttribute(temp));
    ans = temp2;
    XMLString::release(&temp2);
  }
  else {
    ans = "";
  }
  return ans;
}

int xmlParser::getChildCount(const char* parentTag, int parentIndex, const char* childTag) {
  XMLCh* temp = XMLString::transcode(parentTag);
  DOMNodeList* list = m_doc->getElementsByTagName(temp);
  XMLString::release(&temp);
  DOMElement* parent = dynamic_cast<DOMElement*>(list->item(parentIndex));
  DOMNodeList* childList = parent->getElementsByTagName(XMLString::transcode(childTag));
  return (int)childList->getLength();
}

std::string xmlParser::getParentAttribute(const char* parentTag, const char* attributeTag) {
  std::string ans;
  XMLCh* temp = XMLString::transcode(parentTag);
  DOMNodeList* list = m_doc->getElementsByTagName(temp);
  XMLString::release(&temp);
  DOMElement* parent = dynamic_cast<DOMElement*>(list->item(0));
  if (parent) {
    temp = XMLString::transcode(attributeTag);
    char* temp2 = XMLString::transcode(parent->getAttribute(temp));
    ans = temp2;
    XMLString::release(&temp2);
  }
  else {
    ans = "";
  }
  return ans;
}


std::string xmlParser::getRoot() {
  DOMElement* elementRoot = m_doc->getDocumentElement();
  char* temp2 = XMLString::transcode(elementRoot->getTagName());
  std::string ans = temp2;
  XMLString::release(&temp2);
  return ans;
}

std::vector<std::string> xmlParser::parseXML(){
  if (getRoot() == "create") {
    return parseCreateXML();
  } else if (getRoot() == "transactions") {
    return parseTransactionsXML();
  }
  std::vector<std::string> empty;
  return empty;
}

std::vector<std::string> xmlParser::parseCreateXML(){
  std::vector<std::string> ans;
  std::string value;
  int accountNum = this->getChildCount("create", 0, "account");
  int symbolAccountNum = 0;
  for (int i = 0; i < this->getChildCount("create", 0, "symbol"); i++) {
    symbolAccountNum += this->getChildCount("symbol", i, "account");
  }
  accountNum -= symbolAccountNum;
  for (int i = 0; i < accountNum; i++) {
    ans.push_back("newUser");
    value = this->getChildAttribute("create", 0, "account", i, "id");
    ans.push_back(value);
    value = this->getChildAttribute("create", 0, "account", i, "balance");
    ans.push_back(value);
  }
  for (int i = 0; i < this->getChildCount("create", 0, "symbol"); i++) {
    ans.push_back("newSymbol");
    value = this->getChildAttribute("create", 0, "symbol", i, "sym");
    ans.push_back(value);
    for (int j = 0; j < this->getChildCount("symbol", i, "account"); j++) {
      value = this->getChildAttribute("symbol", i, "account", j, "id");
      ans.push_back(value);
      value = this->getChildValue("symbol", i, "account", j);
      ans.push_back(value);
    }
  }

  return ans;
}

std::vector<std::string> xmlParser::parseTransactionsXML(){
  std::vector<std::string> ans;
  ans.push_back("transaction id"); 
  std::string value;
  value = this->getParentAttribute("transactions", "id");
  ans.push_back(value);
  for (int i = 0; i < this->getChildCount("transactions", 0, "order"); i++) {
    ans.push_back("newOrder");
    value = this->getChildAttribute("transactions", 0, "order", i, "sym");
    ans.push_back(value);
    value = this->getChildAttribute("transactions", 0, "order", i, "amount");
    ans.push_back(value);
    value = this->getChildAttribute("transactions", 0, "order", i, "limit");
    ans.push_back(value);
  }
  for (int i = 0; i < this->getChildCount("transactions", 0, "query"); i++) {
    ans.push_back("newQuery");
    value = this->getChildAttribute("transactions", 0, "query", i, "id");
    ans.push_back(value);
  }
  for (int i = 0; i < this->getChildCount("transactions", 0, "cancel"); i++) {
    ans.push_back("newCancel");
    value = this->getChildAttribute("transactions", 0, "cancel", i, "id");
    ans.push_back(value);
  }

  return ans;
}
