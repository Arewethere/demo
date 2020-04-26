#ifndef __EVENTBASE_H__
#define __EVENTBASE_H__

#include<sys/epoll.h>
#include<functional>
/*
Eventbase类：
与一个文件描述符绑定，并有该文件描述符发生事件后的相关处理函数。
*/
class Eventbase
{


public:
    Eventbase(int fd);
    ~Eventbase();

    using Callback=std::function<void()>;

    //设置相应的回调函数
    void Set_read_cb(Callback cb){Read_cb=cb;}
    void Set_write_cb(Callback cb){Write_cb=cb;}
    void Set_close_cb(Callback cb){Close_cb=cb;}
    //设置关注可读事件
    void Set_event_in(){event_|=EPOLLIN;}
    //设置关注可写事件
    void Set_event_out(){event_|=EPOLLOUT;}
    //设置取消可读
    void Set_disable_in(){event_&=~EPOLLIN;}
    //设置取消可写
    void Set_disable_out(){event_&=EPOLLOUT;}
    //返回活跃的事件
    void Return_revent(int revent){revent_=revent;}

    //用于处理返回的活跃事件
    void HandleEvent();

    //获取该eventbase的文件描述符
    int getFd_(){return fd_;}   
    int getEvent_(){return event_;} 

    //判断是否关注了可写事件
    bool IsWriting()const{return event_&EPOLLOUT;}
private:
    //绑定的一个唯一文件描述符
    int fd_;

    //epoll中的事件
    int event_;

    //返回的活跃事件

    int revent_;

    //定义有读事件时的回调函数
    Callback Read_cb;
    //定义有写事件时的回调函数
    Callback Write_cb;
    //定义关闭文件描述符的回调函数
    Callback Close_cb;  
 
};






#endif
