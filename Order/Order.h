#ifndef ORDER_H
#define ORDER_H

#include <chrono>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <iomanip> 
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>


#include "/usr/include/mysql/mysql.h"

class Order {
   public:
    std::string dishName;
    std::chrono::system_clock::time_point date;
    int oilCount;
    int count;
    int fryingLvl;
    std::string orderID;

    Order();
    Order(const std::string &dishName,
          const std::chrono::system_clock::time_point &date,
          const int &oilCount, const int &count,
          const std::string &RestNameOrderID, const int &fryingLvl);
    void firstPage();
    void fillOrderData(std::string currentUserID);
    void takeOrder(std::string currentUserID, Order order);
    std::pair<MYSQL_RES *, int> fetchMenu(std::string currentUserID);
    MYSQL_ROW getDishByID(std::string currentUserID, std::string dishName);
    void centeredText(const std::string &text ) ;
    int getTerminalWidth( ) ;
    void header();
};
#endif