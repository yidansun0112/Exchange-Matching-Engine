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

void * oneRound(void * information){
  Thread_info *info=(Thread_info *)information;
  const char * hostname=info->hostname;
  const char * filename=info->filename;
  Client client(hostname,filename);
  client.run();
  return NULL;
}

int main(int argc, char **argv){
  if(argc!=4){
    cerr<<"FORMAT: ./client hostname filename round"<<endl;
    exit(EXIT_FAILURE);
  }
  const char * hostname=argv[1];
  const char * filename=argv[2];
  int round=atoi(argv[3]);
  if(round==1){
    Client client(hostname,filename);
    client.run();
    return 0;
  }else{
    Client client1(hostname,"create.txt");
    client1.run();
    Thread_info info(hostname,filename);
    int i=0;
    while(i<round){
      pthread_t thread;
      pthread_create(&thread,NULL,oneRound,&info);
      i++;
    }
    Client client2(hostname,"cancel.txt");
    client2.run();
    return 0;
  }
}
