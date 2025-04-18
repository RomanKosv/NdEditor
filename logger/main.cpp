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

static BaseLogSystem log_sys{R"(C:\Users\Public\NdEditor\logs)"};
static decltype(log_sys.log)& LOG = log_sys.log;
static decltype(log_sys.input)& IN = log_sys.input;

int main()
{
    LOG<<11<<12<<"aaaa"<< '\n';
    AddPrefix<decltype(LOG)&> local_log{LOG, "[local]"};
    local_log<<12<<123<<"qwwe"<< '\n';
    int a,b;
    string s;
    IN>>a>>b>>a;
    // getline(INP, s);
    IN>>a;
// >>>>>>> origin/logger
    return 0;
}
