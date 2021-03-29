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

std::string xmlPrinter::createCreateAccountXML(int userId, std::string msg) {
  std::string ans;
  std::string idString = std::to_string(userId);
  if (msg == "success") {
    ans += "  <created id=\"";
    ans += idString;
    ans += "\"/>\n";
  } else {
    ans+="  <error id=\"";
    ans += idString;
    ans += "\">";
    ans += msg;
    ans += "</error>\n";
  }
  return ans;
}
std::string xmlPrinter::createCreateSymbolXML(std::string symbol, int userId, std::string msg) {
  std::string ans;
  std::string idString = std::to_string(userId);
  if (msg == "success") {
    ans += "  <created sym=\"";
    ans += symbol;
    ans += "\" id=\"";
    ans += idString;
    ans +="\"/>\n";
  } else {
    ans += "  <error sym=\"";
    ans += symbol;
    ans += "\" id=\"";
    ans += idString;
    ans += "\">";
    ans += msg;
    ans += "</error>\n";
  }
  return ans;
}

std::string xmlPrinter::createOrderXML(std::string symbol, int amount, double limit, int transId, std::string msg) {
  std::string ans;
  std::string aString = std::to_string(amount);
  std::string lString = std::to_string(limit);
  std::string idString = std::to_string(transId);
  if (msg.substr(0, 7) == "success"){
    std::string idString = msg.substr(14);
    ans += "  <opened sym=\"";
    ans += symbol;
    ans += "\" amount=\"";
    ans += aString;
    ans += "\" limit=\"";
    ans += lString;
    ans += "\" id=\"";
    ans += idString;
    ans += "\"/>\n";
  } else {
    ans += "  <error sym=\"";
    ans += symbol;
    ans += "\" amount=\"";
    ans += aString;
    ans += "\" limit=\"";
    ans += lString;
    ans += "\">";
    ans += msg;
    ans += "</error>\n";
  }
  return ans;
}
std::string xmlPrinter::createQueryXML(int transId,std::vector<std::string> msg) {
  std::string ans;
  std::string idString = std::to_string(transId);
  ans += "  <status id=\"";
  ans += idString;
  ans += "\">\n";
  if (msg.size() == 0) {
      ans += "    <error/>\n";
  }
  for (size_t i = 0; i < msg.size(); i++) {
    ans += "    <";
    ans += msg[i];
    ans += "/>\n";
    }
  ans += "  </status>\n";
  return ans;
}

std::string xmlPrinter::createCancelXML(int transId, std::vector<std::string> msg) {
  std::string ans;
  std::string idString = std::to_string(transId);
  ans += "  <canceled id=\"";
  ans += idString;
  ans += "\">\n";
  if (msg.size() == 0) {
      ans += "    <error>This transaction id does not belong to you.</error>\n";
  }

  for (size_t i = 0; i < msg.size(); i++) {
    ans += "    <";
    ans += msg[i];
    ans += "/>\n";
  }
  ans += "  </canceled>\n";
  return ans;
}
 
