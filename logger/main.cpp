#include <iostream>
#include "basic_loggers.h"
#include "logger.h"

using namespace std;


int main()
{
    LOG<<11<<12<<"aaaa";
    AddPrefix<decltype(LOG)&> local_log{LOG, "[local]"};
    local_log<<12<<123<<"qwwe";
    int a,b;
    string s;
    INPUT>>a>>b>>a;
    // getline(INP, s);
    INPUT>>a;
    return 0;
}
