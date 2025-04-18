#ifndef BASIC_LOGGERS_H
#define BASIC_LOGGERS_H

#include "logger.h"

struct with_input_prefix{
    template<typename Log, typename Obj>
    decltype(auto) operator()(Log& log, Obj obj){
        return log<<"[INPUT]"<<obj;
    }
};

struct BaseLogSystem {
    AddOverview<FileLog> save_log;
    TwoLogs<TimePref<AddPrefix<FileLog&>>, decltype(cout)&> log;
    InputLogger<MapLog<decltype(log.log1)&, with_input_prefix>> input;
    BaseLogSystem(filesystem::path dir)
        : save_log{{dir, FileLog::simple_time_path()}},
        log{{{save_log.log, "\n"}}, {cout}},
        input{{log.log1, {}}, cin}
    {
    }
};


#endif // BASIC_LOGGERS_H
