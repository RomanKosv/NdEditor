#include <iostream>
#include "logger.h"

using namespace std;

static AddOverview<FileLog> log_f{{FileLog::simple_time_path("tests")}};
static auto log = TimePref{startOnLine(TwoLogs<decltype(cout)&, decltype(log_f)&>{cout, log_f})};
static InputLogger<AddPrefix<CycleLog<AddPrefix<FileLog&>>>>
    INP{
        startOnLine<CycleLog<AddPrefix<FileLog&>>>(
            {
                {log_f.log, "[input]"}
            }
            ),
        cin
    };
int main()
{
    log<<11<<12<<"aaaa";
    AddPrefix<decltype(log)&> local_log{log, "[local]"};
    local_log<<12<<123<<"qwwe";
    int a,b;
    string s;
    INP>>a>>b;
    // getline(INP, s);
    INP>>a;
    return 0;
}
