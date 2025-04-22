#include "logger.h"

string get_date_and_time_(char sep) {
    auto tm = time(nullptr);
    char string_t[20];
    strftime(data(string_t), 20, "%F %H-%M-%S", localtime(&tm));
    string str{string_t, 19};
    replace(str.begin(), str.end(), '-', sep);
    return str;
}

string get_time_(char sep) {
    auto tm = time(nullptr);
    char string_t[9];
    strftime(data(string_t), 9, "%H-%M-%S", localtime(&tm));
    string str{string_t, 8};
    replace(str.begin(), str.end(), '-', sep);
    return str;
}
