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
  char buf[65535];
  int size;
  if ((size = recv(client_fd, buf, sizeof(buf), 0)) == -1) {
    return;
  }
  std::vector<char> requestXML(buf, buf + size);
  
}
