#include "xmlPrinter.h"

std::string xmlPrinter::createRequestXML(std::string input) {
  std::string ans;
  std::istringstream buf(input);
  std::istream_iterator<std::string> beg(buf), end;
  std::vector<std::string> tokens(beg, end);
  if (tokens[0] == "create") {
    return getCreateXML(tokens);
  } else if (tokens[0] == "transactions") {
    return getTransactionXML(tokens);
  }
  return ans;
}

std::string xmlPrinter::createHeader() {
  return "\"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\"\n";
}

std::string xmlPrinter::getCreateXML(std::vector<std::string> tokens) {
  std::string ans;
  ans +="<create>\n";
  size_t i = 1;
  size_t size = tokens.size();
  // create new user in this request
  if (tokens[i] == "user") {
    i++;
    while (i < size && tokens[i] != "symbol") {
      ans +="  <account id=\"";
      ans += tokens[i++];
      ans += "\" balance=\"";
      ans += tokens[i++];
      ans += "\"/>\n";
    }
  }
  // create symbol
  while (i != size) {
    if (tokens[i] == "symbol") {
      i++;
      ans += "  <symbol sym=\"";
      ans += tokens[i++];
      ans += "\">\n";
      while (i < size && tokens[i] != "symbol") {
        ans += "    <account id=\"";
        ans += tokens[i++];
        ans += "\">";
        ans += tokens[i++];
        ans += "</account>\n";
      }
      ans+="  </symbol>\n";
    } else {
      break;
    }
  }
  ans +="</create>\n";
  return ans;
}

std::string xmlPrinter::getTransactionXML(std::vector<std::string> tokens) {
  std::string ans;
  ans += "<transactions id=\"";
  ans += tokens[1];
  ans += "\">\n";
  size_t i = 2;
  size_t size = tokens.size();
  while (i != size) { 
    // create new user in this request
    if (tokens[i] == "order") {
      i++;
      ans +="  <order sym=\"";
      ans += tokens[i++];
      ans += "\" amount=\"";
      ans += tokens[i++];
      ans += "\" limit=\"";
      ans += tokens[i++];
      ans += "\"/>\n";
    } else if (tokens[i] == "query") {
      i++;
      ans += "  <query id=\"";
      ans += tokens[i++];
      ans += "\"/>\n";   
    } else if (tokens[i] == "cancel"){
      i++;
      ans += "  <cancel id=\"";
      ans += tokens[i++];
      ans += "\"/>\n";   
    } else {
      break;
    }
  }
  ans +="</transactions>\n";
  return ans;
}

std::string xmlPrinter::createCreateResponseXML(std::string input) {
  std::string ans;
  
  return ans;
}

std::string createTransactionResponseXML(std::string) {
  std::string ans;
  return ans;
}
