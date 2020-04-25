/*
将loop类封装成一个线程，这个线程运行着loop类的start_loop函数
*/

#ifndef __THREADLOOP_H__
#define __THREADLOOP_H__
#include"loop.h"
#include<thread>

class Threadloop{

public:
    Threadloop();
    ~Threadloop();
    //从threadloop中取出loop
    std::shared_ptr<Loop>Get_loop_from_threadloop(){
        return loop_ptr;
    }
private:
    //拥有一个loop的指针，在构造函数中初始化
    std::shared_ptr<Loop>loop_ptr;
    std::thread td_;
};


#endif