#pragma once
#include <string>

namespace logger {
    const int info = 0;
    const int success = 1;
    const int fail = 2;

    void log(const std::string& message, int type = info);
}