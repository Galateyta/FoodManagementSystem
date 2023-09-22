#include "Order.h"

#include <chrono>
#include <iostream>
#include <string>

#include "../Menu/Menu.h"
#include "../Profile/Profile.h"
#include "../calculations/Calculations.h"
#include "../utils/Utils.h"
#include "/usr/include/mysql/mysql.h"

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

int Order::getTerminalWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}
void Order::centeredText(const std::string &text) {
    int terminalWidth = getTerminalWidth();

    int textWidth = text.length();
    int padding = (terminalWidth - textWidth) / 2;
    // cout << "\033[1;31mbold red text\033[0m\n";
    std::cout << std::setw(padding + textWidth)
              << "\033[1;40m" + text + "\033[0m\n"
              << std::endl;
}
void Order::takeOrder(std::string currentUserID, Order order) {
    char server[26] = "sql11.freesqldatabase.com";
    char username[25] = "sql11647725";
    char password[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, server, username, password, database, 0, NULL,
                           0) == NULL) {
        printf("Unable to connect with MySQL server\n");
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
    if (mysql_query(conn, insertQuery.c_str())) {
        std::cout << "Error while trying to insert order" << std::endl;
        mysql_close(conn);
    }

    updateCurrentOilCount(currentUserID, order);
    calculateCoefficientOnOrder(true, currentUserID);
    firstPage();
}

void Order::firstPage() {
    std::string currentUserID = GlobalStates::currentUserID;
    int choice;
    Profile profile;
    Table firstPageTable, choiceTable;

    firstPageTable.add_row({"1. Profile", "2. Order", "3. Exit"});
    firstPageTable[0]
        .format()
        .font_color(Color::yellow)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .width(40);
    std::cout << firstPageTable << std::endl;
    choiceTable.add_row({"Enter Your Choice: "});
    choiceTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << choiceTable;
    std::cout << "\t";
    std::cin >> choice;
    std::cout << std::endl;

    switch (choice) {
        case 1:
            profile.showProfilePage();
            break;
        case 2:
            fillOrderData(currentUserID);
            break;
        case 3:
            centeredText("Thank you!");
            break;
        default:
            centeredText("Select from the options given above");
            firstPage();
    }
}
void Order::fillOrderData(std::string currentUserID) {
    Table choiceTable, countTable;
    std::string dishName;
    std::string count;
    std::chrono::system_clock::time_point orderDate =
        std::chrono::system_clock::now();
    std::string orderID = generateUniqueID();
    system("clear");

    centeredText("--------- Enter details for your Order. ---------");

    choiceTable.add_row({"Enter Dish Name "});
    choiceTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << choiceTable;
    std::cout << "\t";
    std::cout << "_ ";
    std::cin.ignore();
    std::getline(std::cin, dishName);

    countTable.add_row({"Enter Dish Count "});
    countTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << countTable;
    std::cout << "\t";
    std::cout << "_ ";
    std::cin >> count;

    MYSQL_ROW dishRow = getDishByID(currentUserID, dishName);
    int oilCount = std::stoi(dishRow[2]);
    int fryingLevel = std::stoi(dishRow[3]);

    Order newOrder(dishName, orderDate, oilCount, std::stoi(count), orderID,
                   fryingLevel);

    std::string confirmOrderSumbit;
    centeredText("Are you sure you want to submit order: Y/N");
    std::cin >> confirmOrderSumbit;

    if (confirmOrderSumbit == "y" || confirmOrderSumbit == "Y") {
        takeOrder(currentUserID, newOrder);
    } else if (confirmOrderSumbit == "n" || confirmOrderSumbit == "N") {
        centeredText("Order canceled successfully.");
    } else {
        centeredText("Please use correct options - Y/N.");
    }
    firstPage();
}

std::pair<MYSQL_RES *, int> Order::fetchMenu(std::string currentUserID) {
    char server[26] = "sql11.freesqldatabase.com";
    char username[25] = "sql11647725";
    char password[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

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

MYSQL_ROW Order::getDishByID(std::string currentUserID, std::string dishName) {
    std::pair<MYSQL_RES *, int> result = fetchMenu(currentUserID);

    if (result.second != 0) {
        std::cout << "Error while fetching menu." << std::endl;
    }

    MYSQL_RES *menuResult = result.first;

    int num_fields = mysql_num_fields(menuResult);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(menuResult))) {
        std::string rowID = row[0];
        std::string rowDishName = row[1];
        if (rowID.compare(currentUserID) == 0 &&
            rowDishName.compare(dishName) == 0) {
            return row;
        }
    }

    mysql_free_result(menuResult);
    return 0;
}