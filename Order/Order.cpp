#include "Order.h"

#include <chrono>
#include <iostream>
#include <string>

#include "../Profile/Profile.h"
#include "../calculations/Calculations.h"
#include "../utils/Utils.h"
#include "/usr/include/mysql/mysql.h"

int Order::getTerminalWidth()
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

void Order::centeredText(const std::string &text)
{
    int terminalWidth = getTerminalWidth();
    int textWidth = text.length();
    int padding = (terminalWidth - textWidth) / 2;

    std::cout << std::setw(padding + textWidth) << text << std::endl;
}
void Order::header()
{
    system("clear");
    centeredText("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    centeredText("\033[32m FOOD MANAGEMENT\n");
};
Order::Order() {}

Order::Order(const std::string &dishName,
             const std::chrono::system_clock::time_point &date,
             const int &oilCount, const int &count,
             const std::string &RestNameOrderID, const int &fryingLevel)
    : dishName(dishName),
      date(date),
      oilCount(oilCount),
      count(count),
      orderID(RestNameOrderID),
      fryingLvl(fryingLevel) {}

void Order::takeOrder(std::string currentUserID, Order order)
{
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, server, username, password, database, 0, NULL,
                           0) == NULL)
    {
        centeredText("Unable to connect with MySQL server");
        mysql_close(conn);
    }
    std::string insertQuery =
        "INSERT INTO ORDERS(RestaurantID, ID, DishName, Date, OilCount, "
        "Count, "
        "FryingLevel) "
        "VALUES ('" +
        currentUserID + "','" + order.orderID + "','" + order.dishName + "','" +
        timePointToString(order.date) + "','" + std::to_string(order.oilCount) +
        "','" + std::to_string(order.count) + "','" +
        std::to_string(order.fryingLvl) + "')";
    if (mysql_query(conn, insertQuery.c_str()))
    {
        std::cout << "Error while trying to insert order" << std::endl;
        mysql_close(conn);
    }

    updateCurrentOilCount(currentUserID, order);
    calculateCoefficientOnOrder(true, currentUserID);
    firstPage();
}

void Order::firstPage()
{
    std::string currentUserID = GlobalStates::currentUserID;
    int choice;
    Profile profile;
    header();
    std::cout << "\t * Press 1 to return to Profile screen. " << std::endl;
    std::cout << "\t * Press 2 to add Order. " << std::endl;
    std::cout << "\t * Press 3 to EXIT \n\n";
    std::cout << "\t Enter your choice: ";

    std::cin >> choice;

    switch (choice)
    {
    case 1:
        profile.showProfilePage();
        break;
    case 2:
        fillOrderData(currentUserID);
        break;
    case 3:
        header();
        centeredText(" Thank you! ");
        break;
    default:

        header();
        centeredText(" Select from the options given above ");
        firstPage();
    }
}
void Order::fillOrderData(std::string currentUserID)
{
    std::string dishName;
    std::string count;
    std::chrono::system_clock::time_point orderDate =
        std::chrono::system_clock::now();
    std::string orderID = generateUniqueID();
    header();
    centeredText(" --------- Enter details for your Order. --------- ");

    centeredText("Enter Dish name");
    std::cout << "_ ";
    std::cin >> dishName;
    system("clear");
    centeredText("Enter Dish count");
    std::cout << "_ ";
    std::cin >> count;
    MYSQL_ROW dishRow = getDishByID(currentUserID, dishName);
    int oilCount = std::stoi(dishRow[2]);
    int fryingLevel = std::stoi(dishRow[3]);

    Order newOrder(dishName, orderDate, oilCount, std::stoi(count), orderID,
                   fryingLevel);

    std::string confirmOrderSumbit;
    header();
    centeredText("Are you sure you want to submit order: Y/N");
    std::cin >> confirmOrderSumbit;

    if (confirmOrderSumbit == "y" || confirmOrderSumbit == "Y")
    {
        takeOrder(currentUserID, newOrder);
    }
    else if (confirmOrderSumbit == "n" || confirmOrderSumbit == "N")
    {
        header();
        centeredText("Order canceled successfully.");
    }
    else
    {
        header();
        centeredText("Please use correct options - Y/N.");
    }
    firstPage();
}

std::pair<MYSQL_RES *, int> Order::fetchMenu(std::string currentUserID)
{
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL *conn = mysql_init(NULL);
    // Connecting to database
    if (mysql_real_connect(conn, server, username, password, database, 0, NULL,
                           0) == NULL)
    {
        centeredText("Unable to connect with MySQL server\n");
        return std::make_pair(nullptr, 1);
    }

    // Connecting to table
    if (mysql_query(conn, "SELECT * FROM MENU"))
    {
        centeredText("Query execution error.");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    // Getting the result
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == nullptr)
    {
        centeredText("Result fetching error.");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    return std::make_pair(result, 0);
}

MYSQL_ROW Order::getDishByID(std::string currentUserID, std::string dishName)
{
    std::pair<MYSQL_RES *, int> result = fetchMenu(currentUserID);

    if (result.second != 0)
    {
        header();
        centeredText("Error while fetching menu.");
    }

    MYSQL_RES *menuResult = result.first;

    int num_fields = mysql_num_fields(menuResult);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(menuResult)))
    {
        std::string rowID = row[0];
        std::string rowDishName = row[1];
        if (rowID.compare(currentUserID) == 0 &&
            rowDishName.compare(dishName) == 0)
        {
            return row;
        }
    }

    mysql_free_result(menuResult);
    return 0;
}