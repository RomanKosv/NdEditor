#ifndef DEBUG_HH
#define DEBUG_HH

#include <string>
#include <iostream>
using namespace std;
struct Log{
    bool flag;
    string prefix;
    template<typename T>
    void print(T info, string end="\n"){
        if(flag){
            cout<<prefix;
            cout<<info;
            cout<<end;
        }
    }
};

#endif // DEBUG_HH
