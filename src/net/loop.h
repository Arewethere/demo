/*
Loop类主要是进行事件循环
循环Epoller中的Poll函数，并执行
*/
#ifndef __LOOP_H__
#define __LOOP_H__

#include<memory>
#include"epoller.h"

class Loop{
public:
    Loop();
    //~Loop();

    void loop();
    void start_loop();
    //直接调用epoller的相关函数
    void AddEvent_loop(std::shared_ptr<Eventbase>base){
        epoller_ptr->addEvent(base);
    }

    void ModEvent_loop(std::shared_ptr<Eventbase>base){
        epoller_ptr->modEvent(base);
    }

    void DelEvent_loop(std::shared_ptr<Eventbase>base){
        epoller_ptr->delEvent(base);
    }
    //退出事件循环
    void Quit(){
        quit_=true;
    }





private:
    //是否退出的标志
    bool quit_; 
    //需要包含一个epoller指针，在构造函数中初始化
    std::shared_ptr<Epoller>epoller_ptr;



};



#endif