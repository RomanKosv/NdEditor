#ifndef LOGGER_H
#define LOGGER_H

#include <cassert>
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <filesystem>

using namespace std;

string get_date_and_time(char sep = '-') {
    auto tm = time(nullptr);
    char string_t[20];
    strftime(data(string_t), 20, "%F %H-%M-%S", localtime(&tm));
    string str{string_t, 19};
    replace(str.begin(), str.end(), '-', sep);
    return str;
}

string get_time(char sep = '-') {
    auto tm = time(nullptr);
    char string_t[9];
    strftime(data(string_t), 9, "%H-%M-%S", localtime(&tm));
    string str{string_t, 8};
    replace(str.begin(), str.end(), '-', sep);
    return str;
}

template<typename Log>
class TimePref{
public:
    Log log;
    string pref = "[";
    string postf = "]";
    TimePref(Log l) : log{l} {
    }
};

template<typename L>
decltype(auto) operator <<(TimePref<L> log, auto obj) {
    return log.log<<log.pref<< get_time(':') << log.postf << obj;
}

template<typename Log>
struct CycleLog {
    Log log;
};

template<typename Log>
CycleLog<Log>& operator <<(CycleLog<Log> log, auto obj) {
    log.log << obj;
    return log;
}

class FileLog{
public:
    ofstream file;
    FileLog(filesystem::path dir, filesystem::path name) {
        if (!filesystem::exists(dir)) filesystem::create_directories(dir);
        //cout << dir/name <<endl;
        file.open(dir/name);
        assert(file.is_open());
    }
    FileLog(filesystem::path path) : FileLog(path.parent_path(), path.filename()) {
    }

    static filesystem::path simple_time_path(){
        return filesystem::path("log")/(get_date_and_time()+".txt");
    }

    static filesystem::path simple_time_path(string theme) {
        return filesystem::path("log")/theme/(get_date_and_time()+".txt");
    }

    ~FileLog() {
        file.close();
    }
};

FileLog& operator <<(FileLog& log, auto obj) {
    ofstream &f = log.file;
    f<<obj;
    return log;
}

template<typename Log>
struct AddPrefix {
    Log log;
    string prefix;
};

template<typename Log>
decltype(auto) operator <<(AddPrefix<Log> log, auto obj) {
    return log.log<<log.prefix << obj;
}

template<typename Log1, typename Log2>
struct TwoLogs{
    Log1 log1;
    Log2 log2;
};

template<typename Log, typename Obj>
struct get_log_rezult{
    decltype(auto) operator ()(Log l1, Obj o){
        decltype(l1<<o) rez = l1<<o;
        return rez;
    }
};

template<typename Log, typename Obj>
using log_rezult = result_of<get_log_rezult<Log, Obj> (Log, Obj)>::type;

template<typename Log1, typename Log2, typename Obj>
decltype(auto) operator <<(TwoLogs<Log1, Log2>& log_, Obj obj) {
    return TwoLogs<decltype(log_.log1<<obj), decltype(log_.log2<<obj)>{log_.log1 << obj, log_.log2 << obj};
}

template<typename Log, typename Obj>
decltype(auto) operator <<(Log log, Obj obj) {
    Log& ref = log;
    return ref << obj;
}

template<typename Log>
AddPrefix<Log> startOnLine(Log log) {
    return {log, "\n"};
}

#endif // LOGGER_H
