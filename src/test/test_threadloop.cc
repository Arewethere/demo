#include<iostream>
#include"threadloop.h"
#include<unistd.h>
#include<memory>
using namespace std;

int main(){
    //主线程结束后子线程会默认结束
   // Threadloop threadloop;
    //Threadloop threadloop2;
    Threadloop*loop1=new Threadloop;
    //Threadloop*loop2=new Threadloop;
    //sleep(10);
    //std::shared_ptr<Loop>loop_ptr=threadloop.Get_loop_from_threadloop();
    //loop_ptr->Quit();
    //while(1);
    delete loop1;
    cout<<"主线程结束了"<<endl;

}