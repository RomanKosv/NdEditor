#ifndef BASIC_LOGGERS_H
#define BASIC_LOGGERS_H

#include "logger.h"

static AddOverview<FileLog> SAVE_LOG{{filesystem::path("log")/FileLog::simple_time_path()}};

static FileLog& FILE_LOG = SAVE_LOG.log;

static decltype(cout)& CONSOLE_LOG = cout;

static TimePref<AddPrefix<decltype(FILE_LOG)&>> BASE_FILE_LOG{
    startOnLine<decltype(FILE_LOG)&>(
        FILE_LOG
        )
};

static decltype(CONSOLE_LOG)& BASE_CONSOLE_LOG = CONSOLE_LOG;

static TwoLogs<decltype(BASE_FILE_LOG)&, decltype(BASE_CONSOLE_LOG)&> LOG{
    BASE_FILE_LOG,
    BASE_CONSOLE_LOG
};

struct with_input_prefix{
    template<typename Log, typename Obj>
    decltype(auto) operator()(Log& log, Obj obj){
        return log<<"[INPUT]"<<obj;
    }
};

static decltype(auto) INPUT = InputLogger<MapLog<decltype(BASE_FILE_LOG)&, with_input_prefix>>{
    MapLog<decltype(BASE_FILE_LOG)&, with_input_prefix>{
        BASE_FILE_LOG,
        with_input_prefix{}
    },
    cin
};

static void log_file_in_dir(filesystem::path path) {
    SAVE_LOG.log = {path/FileLog::simple_time_path()};
}

#endif // BASIC_LOGGERS_H
