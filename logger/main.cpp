#include <iostream>
#include "logger.h"

using namespace std;

int main()
{
    FileLog log_f{FileLog::simple_time_path("tests")};
    auto log = TimePref{startOnLine<TwoLogs<decltype(cout)&, FileLog&>>(TwoLogs<decltype(cout)&, FileLog&>{cout, log_f})};
    log<<11<<12<<"aaaa";
    AddPrefix<decltype(log)&> local_log{log, "[local]"};
    local_log<<12<<123<<"qwwe";
    return 0;
}
