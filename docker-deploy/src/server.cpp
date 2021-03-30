#include "server.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void Server::createTables(){
  try{
      Database db;
      db.openDatabase();
      db.createTables();
      db.disconnect();
   }catch (const exception &e){
      return;
   }
}

void Server::setUpListener(){
  struct addrinfo hints, *ai;
  int rv;
  int yes = 1;
  // get us a socket and bind it
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if ((rv = getaddrinfo(NULL, port_num, &hints, &ai)) != 0) {
    fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
    exit(1);
  }
  if ((listener = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) == -1) {
    perror("client: socket");
  }
  setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  if (bind(listener, ai->ai_addr, ai->ai_addrlen) < 0) {
    close(listener);
  }

  // listen
  if (listen(listener, 100) == -1) {
    perror("listen");
    exit(3);
  }
  freeaddrinfo(ai); // all done with this
}

void Server::run(){
  createTables();
  setUpListener();
  // vector<int> fds;
  // int i=0;
  while(1) {
    try{
      int client_fd;
      struct sockaddr_storage socket_addr;
      std::string ip;
      socklen_t socket_addr_len = sizeof(socket_addr);
      pthread_mutex_lock(&mutex);
      if (( client_fd = accept(listener, (struct sockaddr *)&socket_addr, &socket_addr_len)) == -1) {
        continue;
      }
      //fds.push_back(client_fd);
      pthread_t thread;
      pthread_create(&thread,NULL,handleRequest,&client_fd);
      cout<<"waiting for next connect"<<endl;
      //i++;
    }catch(exception &e){
      cerr<<e.what()<<endl;
    }
  }
}

void Server::sendString(int client_fd,string message){
  send(client_fd,message.data(),message.size()+1,0);
}

void * Server::handleRequest(void * info){
  Database db;
  Server server;
  db.openDatabase();
  int *client_info=(int*)info;
  int client_fd=*client_info;
  pthread_mutex_unlock(&mutex);
  while(1){
    char message[65535]={0};
    if(recv(client_fd,message,sizeof(message),0)<=0){
      return NULL;
    }
    string msg(message);
    if(msg=="end"){
      cout<<"end"<<endl;
      return NULL;
    }
    std::cout << msg << std::endl;
    std::string xml;
    xml = server.getXML(msg);
    if (xml == "error") {
      server.sendString(client_fd, server.createErrorResponse());
      return NULL;
    }
    xmlParser reader;
    vector<string> result;
    try {
      result = reader.parseXML(xml);
    } catch(...) {
      server.sendString(client_fd, server.createErrorResponse());
      return NULL;
    }
    string response=server.executeParserResult(result,db);
    if (response == "error") {
      server.sendString(client_fd, server.createErrorResponse());
    } else {
      server.sendString(client_fd,response);
    }
  }
}

std::string Server::executeParserResult(std::vector<std::string> input, Database &db) {
  if (input.size() == 0) {
    return "error";
  }
  if (input[0] == "transaction id") {
    return executeTransactionsResult(input,db);
  } else if (input[0] == "newUser" || input[0] == "newSymbol"){
    return executeCreateResult(input,db);
  } else {
    return "error";
  }
}

std::string Server::executeTransactionsResult(std::vector<std::string> input, Database &db){
  std::string ans = "<results>\n";
  size_t i = 2;
  int accountId = stoi(input[1]);
  xmlPrinter* printer = new xmlPrinter();
  while (i < input.size()) {
    if (input[i] == "newOrder") {
      i++;
      std::string symbol = input[i++];
      double amount = 0;
      double price = 0;
      try {
        amount = stod(input[i++]);
        price = stod(input[i++]);
      } catch(std::invalid_argument& e) {
        return "error";
      }
      std::string msg = db.createOrder(symbol, amount, price, accountId);
      ans += printer->createOrderXML(symbol, amount, price, accountId, msg);
      continue;
    } else if (input[i] == "newQuery") {
      i++;
      int trans_id = 0;
      try {
        trans_id = stoi(input[i++]);
      } catch(std::invalid_argument& e) {
        return "error";
      }
      std::vector<std::string> msg = db.queryOrder(trans_id, accountId);
      ans += printer->createQueryXML(trans_id, msg);
      continue;
    } else if (input[i] == "newCancel") {
      i++;
      int trans_id = 0;
      try {
        trans_id = stoi(input[i++]);
      } catch(std::invalid_argument& e) {
        return "error";
      }
      std::vector<std::string> msg = db.cancelOrder(trans_id, accountId);
      ans += printer->createCancelXML(trans_id, msg);
      continue;
    }
  }
  ans += "</results>\n";
  return ans;
}

std::string Server::executeCreateResult(std::vector<std::string> input, Database &db){
  std::string ans = "<results>\n";
  size_t i = 0;
  xmlPrinter* printer = new xmlPrinter();
  while (i < input.size()) {
    if (input[i] == "newUser") {
      i++;
      int id = 0;
      double balance = 0;
      try {
        id = stoi(input[i++]);
        balance = stod(input[i++]);
      } catch(std::invalid_argument& e) {
        return "error";
      }
      std::string msg = db.createAccount(id, balance);
      ans += printer->createCreateAccountXML(id, msg);
      continue;
    } else if (input[i] == "newSymbol") {
      i++;
      std::string symbol = input[i++];
      while (i < input.size() && input[i]!= "newSymbol") {
        int id = 0;
        int share = 0;
        try {
          id = stoi(input[i++]);
          share = stoi(input[i++]);
        } catch(std::invalid_argument& e) {
          return "error";
        }
        std::string msg = db.createSymbol(symbol, id, share);
        ans += printer->createCreateSymbolXML(symbol,id, msg);
      }
      continue;
    }
  }
  ans += "</results>\n";
  return ans;
}

std::string Server::getXML(std::string msg) {
  std::string errorMsg = "error";
  size_t found = msg.find("\n");
  std::string xml;
  size_t length = 0;
  if (found == std::string::npos) {
    return errorMsg;
  }
  try {
    length = std::stoi(msg.substr(0, found));
  } catch(std::invalid_argument& e) {
    return errorMsg;
  }
  if (found == msg.length() - 1) {
    return errorMsg;   
  } else {
    xml = msg.substr(found + 1);
  }
  if (xml.length() != length) {
    return errorMsg;
  } else {
    return xml;
  }
}

std::string Server::createErrorResponse() {
  std::stringstream ans;
  ans << "<results>\n" <<"  <error>This XML is not valid.</error>\n";
  ans << "</results>\n";
  return ans.str();
}
int main() {
  Server server;
  server.run();
  return 0;
}
