#include<iostream>
#include"eventbase.h"

using namespace std;

void cb(){
    cout<<"hello world"<<endl;
}

int main(){
    Eventbase base(1);
    cout<<base.getFd_()<<endl;
    base.Set_read_cb(cb);
    base.Set_event_in();
    base.Return_revent(EPOLLIN);
    base.HandleEvent();
};
   