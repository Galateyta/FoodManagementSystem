#include <iostream>

#include "/usr/include/mysql/mysql.h"

#include <cstdlib>
#include <unistd.h>
#include <iomanip> 
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

class Analytics {
   public:
    void firstPage();
    void showAnalytics();
    void centeredText(const std::string &text ) ;
    int getTerminalWidth( ) ;
    void header();
};