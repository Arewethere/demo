#include"connection.h"
#include"loop.h"
#include<iostream>
#include<memory>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<ctype.h>
using namespace std;

int main(){
    std::shared_ptr<Loop>looptr(new Loop);
    int fd=9;
    struct sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_port=htons(8888);
  addr.sin_addr.s_addr=htonl(INADDR_ANY);

  Connection conn(fd,looptr,addr,addr);

  cout<<conn.Get_connfd()<<endl;
    

}