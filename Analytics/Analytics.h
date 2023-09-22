#include <iostream>
#include <tabulate/table.hpp>

#include "/usr/include/mysql/mysql.h"

#include <cstdlib>
#include <unistd.h>
#include <iomanip> 
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

using namespace tabulate;
class Analytics {
   public:
    void firstPage();
    void showAnalytics();
    void centeredText(const std::string &text);
    int getTerminalWidth();
};