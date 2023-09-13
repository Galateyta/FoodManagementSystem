#include <chrono>
#include <string>

std::string timePointToString(
    const std::chrono::system_clock::time_point& timePoint);

std::string generateUniqueID();

void updateCurrentOilCount(std::string oilCount);