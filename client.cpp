#include "client.h"
void Client::buildConnection(){
  struct addrinfo hints, *servinfo, *ai;
  int rv;
  int yes = 1;
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
}

void Client::sendString(string message){
  send(server_fd,message.data(),message.size()+1,0);
}

void Client::run(){
  buildConnection();
  sendRequest();
  recvResponse();
}

void Client::sendRequest (){
  xmlPinter printer;
  std::ifstream newfile ("xml.txt");
  if (newfile.is_open()){   //checking whether the file is open
    std::string tp;
    while(getline(newfile, tp)){
      //std::cout<<printer.createRequestXML(tp);
      string request=printer.createRequestXML(tp);
      sendString(request);
    }
    newfile.close(); //close the file object.
  }
}

void Client::recvResponse(){
  while(1){
    char message[65536]={0};
    if(recv(server_fd,message,sizeof(message),0)<=0){
      break;
    }
    cout<<message<<endl;
  }
}

int main(int argc, char **argv){
  if(argc!=2){
    cerr<<"FORMAT: ./client hostname"<<endl;
    exit(EXIT_FAILURE);
  }
  const char * hostname=argv[1];
  Client client(hostname,"12345");
  client.run();
  return 0;
}