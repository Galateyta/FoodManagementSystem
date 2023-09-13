#include <chrono>
#include <string>

#include "/usr/include/mysql/mysql.h"

class Order {
   public:
    std::string dishName;
    std::chrono::system_clock::time_point date;
    int oilCount;
    int count;
    std::string
        orderID;  // This should be in this format - RestName + "_" +  OrderID

    Order();
    Order(const std::string &dishName,
          const std::chrono::system_clock::time_point &date,
          const int &oilCount, const int &count,
          const std::string &RestNameOrderID);
    void firstPage();
    void fillOrderData(std::string currentUserID);
    void takeOrder(Order order);
    std::pair<MYSQL_RES *, int> fetchMenu(std::string currentUserID);
    int getDishByID(std::string currentUserID, std::string dishName);
};
