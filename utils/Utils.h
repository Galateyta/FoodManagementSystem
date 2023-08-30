#include <chrono>
#include <string>

#include "/usr/include/mysql/mysql.h"

std::string timePointToString(
    const std::chrono::system_clock::time_point& timePoint);

std::string generateUniqueID();
