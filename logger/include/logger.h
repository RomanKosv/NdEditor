#pragma once

// <<<<<<< HEAD
// #include <iostream>
// #include <ostream>
// template<typename Logger, typename Obj1>
// concept logger1 = requires (Logger& log, Obj1 obj) {
//     {log<<obj};
// };

// template<typename Logger>
// void log_all(Logger &log) {

// }
// template<typename Logger, typename O1, typename ...Rest> requires logger1<Logger, O1>
// auto log_all(Logger &log, O1 a1, Rest... rest) {
//     log<<a1;
//     log_all<Logger, Rest...>(log, rest...);
// }

// template<typename Logger, typename... Rest>
// concept logger = requires (Logger &log, Rest... params) {
//     {log_all<Logger, Rest...>(log, params...)};
// };

// template<typename L1, typename L2>
// class TwoLoggers{
// public:
//     L1 log1;
//     L2 log2;
// };

// template<typename L1, typename L2>
// void operator <<(TwoLoggers<L1, L2> &log, auto obj){
//     log.log1<<obj;
//     log.log2<<obj;
// }

// static_assert(logger<TwoLoggers<decltype(std::cout), decltype(std::cout)>, int>);
// =======
#include <cassert>
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <filesystem>

using namespace std;

string get_date_and_time_(char sep = '-');

string get_time_(char sep = '-');

template<typename Log>
class TimePref{
public:
    Log log;
    string pref = "[";
    string postf = "]";
    TimePref(Log l) : log{l} {
    }
};

template<typename L, typename Obj>
decltype(auto) operator <<(TimePref<L>& log, Obj obj) {
    return log.log<<log.pref<< get_time_(':') << log.postf << obj;
}

// template<typename Log>
// struct CycleLog {
//     Log log;
// };

// template<typename Log, typename Obj>
// CycleLog<Log>& operator <<(CycleLog<Log>& log, Obj obj) {
//     log.log << obj;
//     return log;
// }

template<typename Log, typename Fun>
struct MapLog{
    Log log;
    Fun fun;
};

template<typename Log, typename Fun, typename Obj>
decltype(auto) operator <<(MapLog<Log, Fun>& log, Obj obj) {
    return MapLog<decltype(log.fun(log.log, obj)), Fun&>{log.fun(log.log, obj), log.fun};
}

class FileLog{
public:
    ofstream file{};
    FileLog(filesystem::path dir, filesystem::path name) {
        if (!filesystem::exists(dir)) filesystem::create_directories(dir);
        //cout << dir/name <<endl;
        file.open(dir/name);
        assert(file.is_open());
    }
    FileLog(filesystem::path path) : FileLog(path.parent_path(), path.filename()) {
    }
    FileLog() {
    }

    FileLog(FileLog&& old) {
        file = std::move(old.file);
    }

    FileLog& operator=(FileLog&& old) {
        file = std::move(old.file);
        return *this;
    }

    static filesystem::path simple_time_path(){
        return get_date_and_time_()+".txt";
    }
    ~FileLog() {
        file.close();
    }

};
template<typename Obj>
FileLog& operator <<(FileLog& log, Obj obj) {
    ofstream &f = log.file;
    f<<obj;
    return log;
}

template<typename Log>
struct AddPrefix {
    Log log;
    string prefix;
};

template<typename Log, typename Obj>
decltype(auto) operator <<(AddPrefix<Log>& log, Obj obj) {
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

// template<typename Log, typename Obj>
// decltype(auto) operator <<(Log log, Obj obj) {
//     Log& ref = log;
//     return ref << obj;
// }

template<typename Log>
AddPrefix<Log> startOnLine(Log log) {
    return {log, "\n"};
}

template<typename Log, typename Inp = decltype(cin)&>
struct AddOverview{
    Log log;
    Inp in;
    string prefix = "\n##################################\nOverview:\n";
    string postfix = "\n##################################\n";
    ~AddOverview() {
        // in.clear();
        // in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        log << "\nInput overview (one line):" <<'\n';
        string overview;
        getline(in, overview);
        log << prefix << overview.data() << postfix;
    }
};

template<typename Log, typename Obj>
decltype(auto) operator <<(AddOverview<Log>& log, Obj obj){
    return log.log<<obj;
}

template<typename Log, typename Input = decltype(cin)&>
struct InputLogger{
    Log log;
    Input input;
    static InputLogger<Log> stdInput(Log logger) {
        return {logger, cin};
    }
};

template<typename Log, typename Inp, typename Obj>
decltype(auto) operator>>(InputLogger<Log, Inp>& input, Obj &obj) {
    decltype(auto) new_inp = input.input>>obj;
    return InputLogger<decltype(input.log<<obj), decltype(input.input>>obj)>{input.log<<obj, new_inp};
}

template<typename Log, typename Inp, typename Str>
decltype(auto) getline(InputLogger<Log, Inp>& input, Str& str) {
    decltype(auto) new_inp = getline(input.input, str);
    return InputLogger<decltype(input.log<<str), decltype(getline(input.input, str))>{input.log<<str, new_inp};
}

template<typename S>
struct Holder{
    S s;
};

template<typename S, typename Obj>
decltype(auto) operator <<(Holder<S> h, Obj o) {
    return Holder<decltype(h.s<<o)>{h.s<<o};
}

template<typename S, typename O>
decltype(auto) operator >>(Holder<S> h, O& o) {
    return Holder<decltype(h.s>>o)>{h.s>>o};
}

template<typename S, typename O>
decltype(auto) getline(Holder<S> h, O& o) {
    return Holder<decltype(getline(h.s, o))>{getline(h.s, o)};
}

// >>>>>>> origin/logger

// #endif // LOGGER_H
