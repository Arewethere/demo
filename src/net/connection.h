/*
将每一个tcp连接封装成一个类，与相应的connfd对应
*/
#include"iobuffer.h"
#include"eventbase.h"
#include"epoller.h"
#include"loop.h"
#include<sys/socket.h>
#include<memory>

class Connection{

public:
    Connection(int connfd,std::shared_ptr<Loop>loop_ptr,struct sockaddr_in&local_addr,struct sockaddr_in&peer_addr);
    ~Connection();
    using Callback=std::function<void()>;
    //处理消息的回调函数
    using MessageCallback=std::function<void(IOBuffer*)>;

    //得到该连接的文件描述符
    int Get_connfd(){
        return connfd_;
    }

    //处理读、写、关闭事件，就是定义eventbase的三个接口
    void Handread();
    void Handwrite();
    void Handclose();

    //设置建立连接回调函数
    void Set_conn_established_cb(Callback&cb){
        conn_established_cb_=cb;
    }
    //设置消息到达回调函数
    void Set_message_arrvial_cb(MessageCallback&cb){
        message_arrival_cb_=cb;
    }
    //设置消息回复完成回调函数
    void Set_message_reply_cb(Callback&cb){
        message_reply_cb_=cb;
    }
    //设置连接关闭回调函数
    void Set_conn_close_cb(Callback&cb){
        conn_close_cb_=cb;
    }
    //向传入的loop上面注册这个eventbase事件；
    void Register();

    void Send(const void* data,size_t len);
    void Send(const std::string& message);
    void Send(IOBuffer& buffer);

private:
    //连接的那个描述符
    int connfd_;    
    //loop指针
    std::shared_ptr<Loop>loop_ptr_;
    //connfd_对应的Eventbase
    std::shared_ptr<Eventbase>conn_eventbase_ptr_;
    //输入输出缓冲区
    IOBuffer inbuffer_;
    IOBuffer outbuffer_;
    //连接建立回调函数
    Callback conn_established_cb_;
    //消息到达回调函数
    MessageCallback message_arrival_cb_;
    //答复消息完成回调函数
    Callback message_reply_cb_;
    //连接关闭回调函数
    Callback conn_close_cb_;

    //本地的地址结构
    struct sockaddr_in local_addr_;
    //对端的地址结构
    struct sockaddr_in peer_addr_;

};