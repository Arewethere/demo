#include"loop.h"
#include<iostream>

Loop::Loop()
    :quit_(false),
    epoller_ptr(new Epoller)
{
}

void Loop::loop(){
    std::vector<std::shared_ptr<Eventbase>>active_list;
    active_list=epoller_ptr->Poll();
    //取出每一个活跃事件指针并执行
    for(int i=0;i<active_list.size();i++){
        active_list[i]->HandleEvent();
    }
}

void Loop::start_loop(){
    std::cout<<"looping..."<<std::endl;

    while(!quit_){
        loop();
        std::cout<<"is looping"<<std::endl;
    }
}
