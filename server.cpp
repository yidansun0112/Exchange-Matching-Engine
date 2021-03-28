#include "server.h"


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
  setUpListener();
  while(1) {
    int client_fd;
    struct sockaddr_storage socket_addr;
    std::string ip;
    socklen_t socket_addr_len = sizeof(socket_addr);
    if (( client_fd = accept(listener, (struct sockaddr *)&socket_addr, &socket_addr_len)) == -1) {
      continue;
    }
    handleRequest(client_fd);
  }
}

void Server::handleRequest(int client_fd){
  db.openDatabase();
  char buf[65535];
  int size;
  if ((size = recv(client_fd, buf, sizeof(buf), 0)) == -1) {
    return;
  }
  std::string xml = buf;
  //xmlParser* reader = new xmlParser(buf); 
}

std::string Server::executeParserResult(std::vector<std::string> input) {
  if (input.size() == 0) {
    std::string ans;
    return ans;
  }
  if (input[0] == "transaction id") {
    return executeTransactionsResult(input);
  } else {
    return executeCreateResult(input);
  }
}

std::string Server::executeTransactionsResult(std::vector<std::string> input){
  std::string ans = "<results>";
  size_t i = 2;
  int accountId = stoi(input[1]);
  xmlPrinter* printer = new xmlPrinter();
  while (i < input.size()) {
    if (input[i] == "newOrder") {
      i++;
      std::string symbol = input[i++];
      double amount = stod(input[i++]);
      double price = stod(input[i++]);
      std::string msg = db.createOrder(symbol, amount, price, accountId);
      ans += printer->createOrderXML(symbol, amount, price, accountId, msg);
    }
    if (input[i] == "newQuery") {
      i++;
      int trans_id = stoi(input[i++]);
      std::vector<std::string> msg = db.queryOrder(trans_id);
      ans += printer->createQueryXML(trans_id, msg);
    }
    if (input[i] == "newCancel") {
      i++;
      int trans_id = stoi(input[i++]);
      std::vector<std::string> ans = db.cancelOrder(trans_id);
      for (size_t i = 0; i < ans.size(); i++) {
        std::cout<< ans[i] <<std::endl;
      }
    }
  }
  return ans;

}

std::string Server::executeCreateResult(std::vector<std::string> input){
  std::string ans;
  size_t i = 0;
  xmlPrinter* printer = new xmlPrinter();
  while (i < input.size()) {
    if (input[i] == "newUser") {
      i++;
      int id = stoi(input[i++]);
      double balance = stod(input[i++]);
      std::cout<<db.createAccount(id, balance)<<std::endl;
    }
    if (input[i] == "newSymbol") {
      i++;
      std::string symbol = input[i++];
      while (i < input.size() && input[i]!= "newSymbol") {
        int id = stoi(input[i++]);
        int share = stoi(input[i++]);
        std::cout<<db.createSymbol(symbol, id, share);
      }
    }
  }
  return ans;
}

