#include"connection.h"
#include<memory>

Connection::Connection(int connfd,std::shared_ptr<Loop>loop_ptr,struct sockaddr_in&local_addr,struct sockaddr_in&peer_addr):
connfd_(connfd),
loop_ptr_(loop_ptr),
local_addr_(local_addr),
peer_addr_(peer_addr),
conn_eventbase_ptr_(new Eventbase(connfd_))
{
    //设置eventbase的相关接口
    conn_eventbase_ptr_->Set_read_cb(std::bind(&Connection::Handread,this));
    conn_eventbase_ptr_->Set_write_cb(std::bind(&Connection::Handwrite,this));
    conn_eventbase_ptr_->Set_close_cb(std::bind(&Connection::Handclose,this));
    //设置关注可读事件

    conn_eventbase_ptr_->Set_event_in();
}

Connection::~Connection(){
    close(connfd_);
}

void Connection::Register(){
    //向loop上注册eventbase；
    loop_ptr_->AddEvent_loop(conn_eventbase_ptr_);
}

void Connection::Handread(){
    int saved_errno=0;
    ssize_t n=0;
    n=inbuffer_.ReadFd(connfd_,&saved_errno);
    if(n>0){
        if(message_arrival_cb_){
            message_arrival_cb_(&inbuffer_);
        }
    }
    else{
        //如果读到0，说明对端关闭，则关闭这个连接
        Handclose();
    }
}

void Connection::Handwrite(){
    //如果关注了可写事件
    if(conn_eventbase_ptr_->IsWriting()){
    //写到内核缓冲区中，就是从outbuffer中读走
    ssize_t n=write(connfd_,outbuffer_.GetReadablePtr(),outbuffer_.GetReadableSize());
    if(n>0){
        outbuffer_.Retrieve(n);
        //如果没有要发送到内核的数据了
        if(outbuffer_.GetReadableSize()==0){
            //conn_eventbase_ptr_->Set_disable_out();
            conn_eventbase_ptr_->Set_event_in();
            //取消关注写事件
            loop_ptr_->ModEvent_loop(conn_eventbase_ptr_);
            if(message_reply_cb_){
                //同时调用设置的消息回复完成函数
                message_reply_cb_();
            }
        }
    }
    }
}

void Connection::Handclose(){
    //从epoll上删除这个事件
    loop_ptr_->DelEvent_loop(conn_eventbase_ptr_);
    if(conn_close_cb_){
        conn_close_cb_();
    }
}

//往对端发送消息

void Connection::Send(const void*data,size_t len){
    ssize_t n_wrote=0;;
    //当outbuffer为空的时候，直接写到内核缓冲区
    
    if(!conn_eventbase_ptr_->IsWriting()&&outbuffer_.GetReadableSize()==0)
    {
        n_wrote=write(connfd_,data,len);
        if(n_wrote>=0){
            //数据已经写完而且设置了消息回复完成函数
            if((size_t)n_wrote==len&&message_reply_cb_){
                message_reply_cb_();
            }
        }

    }

    //数据没有一次性写入或者缓冲区不为空，就把要写的数据加到缓冲区中
    if((size_t)n_wrote<len){
        outbuffer_.Append(static_cast<const char*>(data)+n_wrote,len-n_wrote);
        //如果没有关注可写事件，则关注可写事件；
        if(!conn_eventbase_ptr_->IsWriting()){
            conn_eventbase_ptr_->Set_event_out();
            loop_ptr_->ModEvent_loop(conn_eventbase_ptr_);
        }
    }

}

void Connection::Send(const std::string& message){
    Send(message.data(),message.size());
}

void Connection::Send(IOBuffer&buffer){
    Send(buffer.GetReadablePtr(),buffer.GetPrependSize());
    buffer.RetrieveAll();
}