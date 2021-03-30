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
      //std::cout<<printer.createRequestXML(tp);
      string request=printer.createRequestXML(tp);
      //cout<<request<<endl;
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

// void Client::sendRequest(string str){
//   xmlPrinter printer;
//   string request=printer.createRequestXML(str);
//   sendString(request);
//   cout<<"before"<<endl;
//   recvResponse();
//   cout<<"after"<<endl;
// }

// void * oneRound(void * information){
//   Thread_info *info=(Thread_info *)information;
//   const char * hostname=info->hostname;
//   const char * filename=info->filename;
//   Client client(hostname,filename);
//   client.buildConnection();
//   client.sendRequest("transactions 54321 order SYM -2 3");
//   cout<<"send"<<endl;
//   //client.sendRequest("transactions 12345 order SYM 2 3");
//   //client.sendRequest("transactions 54321 order SYM 2 3");
//   //client.sendRequest("transactions 12345 order SYM -2 3");
//   client.sendString("end");
//   //client.closefd();
//   return NULL;
// }

// void doCancel(const char * hostname,int round){
//   Client client(hostname,"cancel.txt");
//   client.buildConnection();
//   client.sendRequest("transactions 54321 cancel 0");
//   stringstream ss;
//   ss<<"transactions 12345 cancel "<<4*round-1;
//   client.sendRequest(ss.str());
//   client.sendRequest("end");
//   client.closefd();
// }

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
