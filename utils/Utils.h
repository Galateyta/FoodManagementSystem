#include <chrono>
#include <string>

#include "../Order/Order.h"
#include "/usr/include/mysql/mysql.h"

std::string timePointToString(
    const std::chrono::system_clock::time_point& timePoint);

std::string generateUniqueID();

void updateCurrentOilCount(std::string currentUserID, Order order);

std::pair<float, float> getTotalWastedAndUsedOil(std::string currentUserID);
