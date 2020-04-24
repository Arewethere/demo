#include"epoller.h"
#include<iostream>

#define EPOLL_MAXNUMS 4096
//设置epoll_wait的等待时间为10秒
#define EPOLL_WAIT_TIME 10000

Epoller::Epoller():active_event_contains_(EPOLL_MAXNUMS)
{
    //创建一个epoll
    efd_=epoll_create(1024);

}

Epoller::~Epoller(){
    close(efd_);

}
//添加
void Epoller::addEvent(std::shared_ptr<Eventbase>base){
    struct epoll_event epoll_event_;
    epoll_event_.data.fd=base->getFd_();
    epoll_event_.events=base->getEvent_();
    epoll_ctl(efd_,EPOLL_CTL_ADD,base->getFd_(),&epoll_event_);

    //添加到map中
    fd_eventbase_map_[epoll_event_.data.fd]=base;
}
//修改
void Epoller::modEvent(std::shared_ptr<Eventbase>base){
    struct epoll_event epoll_event_;
    epoll_event_.data.fd=base->getFd_();
    epoll_event_.events=base->getEvent_();
    epoll_ctl(efd_,EPOLL_CTL_MOD,base->getFd_(),&epoll_event_);

}
//删除事件
void Epoller::delEvent(std::shared_ptr<Eventbase>base){
    struct epoll_event epoll_event_;
    epoll_event_.data.fd=base->getFd_();
    epoll_event_.events=base->getEvent_();
    epoll_ctl(efd_,EPOLL_CTL_DEL,base->getFd_(),&epoll_event_);
    //从映射map中删除
    fd_eventbase_map_[epoll_event_.data.fd]=base;
}

//返回活跃的Eventbase列表
std::vector<std::shared_ptr<Eventbase>> Epoller::Poll(){
    //这里时vector代替树组作为参数，所以需要将vector第一个数据的地址传进去

    int active_nums=epoll_wait(efd_,&active_event_contains_[0],active_event_contains_.size(),EPOLL_WAIT_TIME);


    std::vector<std::shared_ptr<Eventbase>>active_eventbase_list_;
    for(int i=0;i<active_nums;i++){

        std::shared_ptr<Eventbase>base_ptr=fd_eventbase_map_[active_event_contains_[i].data.fd];
        active_eventbase_list_.push_back(base_ptr);
    }

    return active_eventbase_list_;

}

