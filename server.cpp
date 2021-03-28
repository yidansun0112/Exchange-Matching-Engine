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

void Server::sendString(int client_fd,string message){
  send(client_fd,message.data(),message.size()+1,0);
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

void Server::executeParserResult(std::vector<std::string> input) {
  if (input.size() == 0) {
    return;
  }
  if (input[0] == "transaction id") {
    executeTransactionsResult(input);
  } else {
    executeCreateResult(input);
  }
}

void Server::executeTransactionsResult(std::vector<std::string> input){
  size_t i = 2;
  int transactionId = stoi(input[1]);
  while (i < input.size()) {
    if (input[i] == "newOrder") {
      i++;
      std::string symbol = input[i++];
      double amount = stod(input[i++]);
      double price = stod(input[i++]);
      std::cout << db.createOrder(symbol, amount, price, transactionId)<<std::endl;
    }
    if (input[i] == "newQuery") {
      i++;
      int id = stoi(input[i++]);
      std::vector<std::string> ans = db.queryOrder(id);
      for (size_t i = 0; i < ans.size(); i++) {
        std::cout << ans[i] <<std::endl;
      }
    }
    if (input[i] == "newCancel") {
      i++;
      int id = stoi(input[i++]);
      std::vector<std::string> ans = db.cancelOrder(id);
      for (size_t i = 0; i < ans.size(); i++) {
        std::cout<< ans[i] <<std::endl;
      }
    }
  }
}

void Server::executeCreateResult(std::vector<std::string> input){
  size_t i = 0;
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
}

