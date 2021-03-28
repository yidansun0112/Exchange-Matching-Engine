#include "client.h"
void Client::buildConnection(){
  struct addrinfo hints, *servinfo, *ai;
  int rv;
  int yes = 1;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  // connect my ip
  if ((rv = getaddrinfo("vcm-18615.vm.duke.edu", port_num, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
  }
  if ((server_fd = socket(servinfo->ai_family, servinfo->ai_socktype,
                       servinfo->ai_protocol)) == -1) {
    perror("client: socket");
    return;
  }

  if (connect(server_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    close(server_fd);
    perror("client: connect");
    return;
  }

  freeaddrinfo(servinfo); 
}


void Client::run(){
  //buildConnection();
 
}
