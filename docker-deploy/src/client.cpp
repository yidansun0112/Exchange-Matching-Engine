#include "client.h"
void Client::buildConnection(){
  struct addrinfo hints, *servinfo;
  int rv;
  //int yes = 1;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  // connect my ip
  if ((rv = getaddrinfo(hostname, port_num, &hints, &servinfo)) != 0) {
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
  cout<<server_fd<<endl;
}

void Client::sendString(string message){
  send(server_fd,message.data(),message.size()+1,0);
}

void Client::run(){
    buildConnection();
    sendRequest();
    close(server_fd);
}

void Client::sendRequest (){
  xmlPrinter printer;
  std::ifstream newfile (filename);
  if (newfile.is_open()){   //checking whether the file is open
    std::string tp;
    while(getline(newfile, tp)){
      string request;
      if (std::string(filename) == "invalid.txt") {
        request = printer.createInvalidXML(tp);
      } else {
        request = printer.createRequestXML(tp);
      }
      sendString(request);
      recvResponse();
    }
    newfile.close(); //close the file object.
  }
  sendString("end");
}

void Client::recvResponse(){
  char message[65536]={0};
  recv(server_fd,message,sizeof(message),0);
  cout<<message<<endl;
}


int main(int argc, char **argv){
  if(argc!=3){
    cerr<<"FORMAT: ./client hostname filename"<<endl;
    exit(EXIT_FAILURE);
  }
  const char * hostname=argv[1];
  const char * filename=argv[2];
  Client client(hostname,filename);
  client.run();
  return 0;
}
