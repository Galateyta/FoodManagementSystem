#include "Order.h"

#include <chrono>
#include <iostream>
#include <string>

#include "../Profile/Profile.h"
#include "../utils/Utils.h"
#include "/usr/include/mysql/mysql.h"

Order::Order() {}

Order::Order(const std::string &dishName,
             const std::chrono::system_clock::time_point &date,
             const int &oilCount, const int &count,
             const std::string &RestNameOrderID)
    : dishName(dishName),
      date(date),
      oilCount(oilCount),
      count(count),
      orderID(RestNameOrderID) {}

void Order::takeOrder(Order order) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, server, username, password, database, 0, NULL,
                           0) == NULL) {
        printf("Unable to connect with MySQL server\n");
        mysql_close(conn);
    }

    std::string insertQuery =
        "INSERT INTO ORDERS(ID, DishName, Date, OilCount, Count) VALUES "
        "('" +
        order.orderID + "' , '" + order.dishName + "' , '" +
        timePointToString(order.date) + "' , '" +
        std::to_string(order.oilCount) + "' , '" + std::to_string(order.count) +
        "')";

    if (mysql_query(conn, insertQuery.c_str())) {
        std::cout << "Error while trying to insert order" << std::endl;
        mysql_close(conn);
    }
    firstPage();
}

void Order::firstPage() {
    int choice;
    Profile profile;
    std::cout << "\t * Press 1 to return to Profile screen. " << std::endl;
    std::cout << "\t * Press 2 to add Order. " << std::endl;
    std::cout << "\t * Press 3 to EXIT \n\n";
    std::cout << "\t Enter your choice: ";

    std::cin >> choice;
    switch (choice) {
        case 1:
            profile.showProfilePage();
            break;
        case 2:
            fillOrderData("KFC_ID");
            break;
        case 3:
            std::cout << "\t\t\t Thank you! \n\n";
            break;
        default:
            std::cout << "\033[2J\033[1;1H";
            std::cout << "\t\t\t Select from the options given above \n";
            fillOrderData("KFC_ID");  // TODO fix
    }
}
void Order::fillOrderData(std::string currentUserID) {
    std::string dishName;
    std::string count;
    std::chrono::system_clock::time_point orderDate =
        std::chrono::system_clock::now();
    std::string orderID = generateUniqueID();

    std::cout
        << "\t\t\t --------- Enter details for your Order. --------- \n\n";

    std::cout << "Enter Dish name" << std::endl;
    std::cout << "_ ";
    std::cin >> dishName;

    std::cout << "Enter Dish count" << std::endl;
    std::cout << "_ ";
    std::cin >> count;
    int oilCount = getDishByID(currentUserID, dishName);
    Order newOrder(dishName, orderDate, oilCount, std::stoi(count), orderID);

    std::string confirmOrderSumbit;
    std::cout << "Are you sure you want to submit order: Y/N" << std::endl;
    std::cin >> confirmOrderSumbit;

    if (confirmOrderSumbit == "y" || confirmOrderSumbit == "Y") {
        takeOrder(newOrder);
    } else if (confirmOrderSumbit == "n" || confirmOrderSumbit == "N") {
        std::cout << "Order canceled successfully." << std::endl;
    } else {
        std::cout << "Please use correct options - Y/N." << std::endl;
    }
}

std::pair<MYSQL_RES *, int> Order::fetchMenu(std::string currentUserID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL *conn = mysql_init(NULL);
    // Connecting to database
    if (mysql_real_connect(conn, server, username, password, database, 0, NULL,
                           0) == NULL) {
        printf("Unable to connect with MySQL server\n");
        return std::make_pair(nullptr, 1);
    }

    // Connecting to table
    if (mysql_query(conn, "SELECT * FROM MENU")) {
        std::cerr << "Query execution error." << std::endl;
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    // Getting the result
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == nullptr) {
        std::cerr << "Result fetching error." << std::endl;
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    return std::make_pair(result, 0);
}

int Order::getDishByID(std::string currentUserID, std::string dishName) {
    std::pair<MYSQL_RES *, int> result =
        fetchMenu(currentUserID);  // change this to real id

    if (result.second != 0) {
        std::cout << "Error while fetching menu." << std::endl;
        return 1;  // Return early in case of error
    }

    MYSQL_RES *menuResult = result.first;

    int num_fields = mysql_num_fields(menuResult);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(menuResult))) {
        std::string rowID = row[0];
        ;
        std::string rowDishName = row[1];
        if (rowID.compare(currentUserID) == 0 &&
            rowDishName.compare(dishName) == 0) {
            return std::stoi(row[2]);
        }
    }

    mysql_free_result(menuResult);
    return 0;
}