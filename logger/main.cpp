#include <iostream>
// <<<<<<< HEAD
// =======
#include "basic_loggers.h"
// >>>>>>> origin/logger
#include "logger.h"

using namespace std;

// <<<<<<< HEAD
// int main()
// {
//     cout << "Hello World!" << endl;
// =======

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
// >>>>>>> origin/logger
    return 0;
}
