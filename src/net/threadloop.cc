#include"threadloop.h"

Threadloop::Threadloop():
loop_ptr(new Loop),
td_(std::thread(&Loop::start_loop,&(*loop_ptr)))
{
}

Threadloop::~Threadloop(){
    //当主线程要结束时，会调用该析构函数，然后阻塞等待join，等待子线程结束
    td_.join();
}