#include"threadpool.h"

//构造函数
Threadpool::Threadpool(int num):
num_(num),
flag_(0)
{}

void Threadpool::threadpool_start(){
    for(int i=0;i<num_;i++){
        std::shared_ptr<Threadloop>lp(new Threadloop);
        threadloopList_.push_back(lp);
        loopList_.push_back(lp->Get_loop_from_threadloop());
    }
}
std::shared_ptr<Loop> Threadpool::Get_loop_from_treadpool(){
    if(flag_>=num_){
        flag_=flag_%num_;
    }
    std::shared_ptr<Loop>temp=loopList_[flag_];
    flag_++;
    return temp;
}
