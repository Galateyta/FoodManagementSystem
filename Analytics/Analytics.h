#include <iostream>
#include <tabulate/table.hpp>

#include "/usr/include/mysql/mysql.h"

using namespace tabulate;
class Analytics {
   public:
    void firstPage();
    void showAnalytics();
    void centeredText(const std::string &text);
    int getTerminalWidth();
};