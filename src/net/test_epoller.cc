#include"epoller.h"
#include"eventbase.h"
#include<iostream>
#include<memory>
using namespace std;

int main(){
    Epoller epoller;
    std::shared_ptr<Eventbase>base(new Eventbase(10));
    base->Set_event_in();

    epoller.addEvent(base);
    epoller.Poll();
    
}