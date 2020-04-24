#include<iostream>
#include"../net/eventbase.h"

using namespace std;

int main(){
    Eventbase base(1);
    cout<<base.getFd_();
};
   