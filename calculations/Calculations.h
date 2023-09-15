#include <iostream>

#include "../Order/Order.h"
#include "../utils/Utils.h"
#include "/usr/include/mysql/mysql.h"
float calculateCoefficientOnOrder(bool post, std::string currentUserID);

float calculateFrequencyOnOrder(bool post, std::string restID,
                                std::string currentUserID);