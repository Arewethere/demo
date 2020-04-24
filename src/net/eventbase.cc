#include"eventbase.h"
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>

Eventbase::Eventbase(int fd):fd_(fd)
{
}
Eventbase::~Eventbase(){
    close(fd_);
}

void Eventbase::HandleEvent(){
    if(revent_&EPOLLIN){
        if(Read_cb)
            Read_cb();
    }
    if(revent_&EPOLLOUT){
        if(Write_cb)
            Write_cb();
    }
}

