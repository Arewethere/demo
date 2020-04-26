#include<iostream>
#include"threadpool.h"
#include<unistd.h>

int main(){
    Threadpool*ptr=new Threadpool(3);
    ptr->threadpool_start();
    sleep(10);
    auto it=ptr->Get_loop_from_treadpool();
    it->Quit();
    delete ptr;

}