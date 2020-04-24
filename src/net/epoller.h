#ifndef __EPOLLER_H__
#define __EPOLLER_H__
/*
Epoller类：
    主要是创建于一个epoll，并将事件挂上；
*/
#include<sys/epoll.h>
#include<unistd.h>
#include<unordered_map>
#include<vector>
#include<memory>
#include"eventbase.h"

class Epoller{
public:
    Epoller();
    ~Epoller();
    //在epoll上添加事件
    void addEvent(std::shared_ptr<Eventbase>base);
    //在epoll上修改事件
    void modEvent(std::shared_ptr<Eventbase>base);
    //删除epoll上事件
    void delEvent(std::shared_ptr<Eventbase>base);

    //封装epoll_wait,返回活跃事件指针
    std::vector<std::shared_ptr<Eventbase>>Poll();



private:
    //epoll的文件描述符
    int efd_;
    //用来盛放活跃的事件，参数是epoll_event，epoll_wait函数的参数
    std::vector<epoll_event>active_event_contains_;
    //文件描述符和eventbase的映射
    std::unordered_map<int,std::shared_ptr<Eventbase>>fd_eventbase_map_;
};


#endif