/*
创建指定数量的线程，并且轮询的取出线程；
*/

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include<vector>
#include"threadloop.h"

class Threadpool{

public:
    Threadpool(int num);
    //创建线程
    void threadpool_start();

    //取出线程函数
    std::shared_ptr<Loop>Get_loop_from_treadpool();

private:
    //指定创建线程数量
    int num_;
    //用于轮询的取出线程中的loop
    int flag_;
    //用于放threadloop
    std::vector<std::shared_ptr<Threadloop>>threadloopList_;
    //用于放每一个线程的loop指针
    std::vector<std::shared_ptr<Loop>>loopList_;

};



#include"threadloop.h"

#endif