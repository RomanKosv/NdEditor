#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <ostream>
template<typename Logger, typename Obj1>
concept logger1 = requires (Logger& log, Obj1 obj) {
    {log<<obj};
};

template<typename Logger>
void log_all(Logger &log) {

}
template<typename Logger, typename O1, typename ...Rest> requires logger1<Logger, O1>
auto log_all(Logger &log, O1 a1, Rest... rest) {
    log<<a1;
    log_all<Logger, Rest...>(log, rest...);
}

template<typename Logger, typename... Rest>
concept logger = requires (Logger &log, Rest... params) {
    {log_all<Logger, Rest...>(log, params...)};
};

template<typename L1, typename L2>
class TwoLoggers{
public:
    L1 log1;
    L2 log2;
};

template<typename L1, typename L2>
void operator <<(TwoLoggers<L1, L2> &log, auto obj){
    log.log1<<obj;
    log.log2<<obj;
}

static_assert(logger<TwoLoggers<decltype(std::cout), decltype(std::cout)>, int>);

#endif // LOGGER_H
