#include "./Utils.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <string>

std::string generateUniqueID() {
    // Unique ID generation algorithm. Source -
    // https://saturncloud.io/blog/algorithm-for-generating-a-unique-id-in-c/
    auto currentTime =
        std::chrono::system_clock::now().time_since_epoch().count();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9999);
    auto randomNumber = dis(gen);

    std::string combinedString =
        std::to_string(currentTime) + std::to_string(randomNumber);

    std::hash<std::string> hasher;
    std::size_t hashedValue = hasher(combinedString);

    std::string uniqueID = std::to_string(hashedValue);

    return uniqueID;
}

std::string timePointToString(
    const std::chrono::system_clock::time_point &timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S",
                  std::localtime(&time));
    return buffer;
}

void updateCurrentOilCount(std::string currentUserID, Order order) {
    char server[26] = "sql11.freesqldatabase.com";
    char username[25] = "sql11647725";
    char password[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        std::cerr << "Unable to connect with MySQL server\n";
        mysql_close(conn);
    }

    std::string selectOilQuery =
        "SELECT * FROM RESTAURANTS WHERE ID = '" + currentUserID + "'";

    if (mysql_query(conn, selectOilQuery.c_str())) {
        std::cerr << "Query execution error: " << mysql_error(conn)
                  << std::endl;
        mysql_close(conn);
    }

    MYSQL_RES *currentOilresult = mysql_store_result(conn);
    if (currentOilresult == nullptr) {
        std::cerr << "Result fetching error." << std::endl;
        mysql_close(conn);
    }
    float currentOil = 0;

    MYSQL_ROW roww;
    while ((roww = mysql_fetch_row(currentOilresult))) {
        currentOil = std::stoi(roww[8]);
    }
    std::string selectRowQuery =
        "SELECT * FROM RESTAURANTS WHERE ID = '" + currentUserID + "'";

    if (mysql_query(conn, selectRowQuery.c_str())) {
        std::cerr << "Query execution error: " << mysql_error(conn)
                  << std::endl;
        mysql_close(conn);
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == nullptr) {
        std::cerr << "Result fetching error." << std::endl;
        mysql_close(conn);
    }
    float uploadCurrentOilCount = currentOil + (order.oilCount * order.count);
    std::string updateRowQuery = "UPDATE RESTAURANTS SET CurrentOilCount = " +
                                 std::to_string(uploadCurrentOilCount) +
                                 " WHERE ID = '" + currentUserID + "'";

    if (mysql_query(conn, updateRowQuery.c_str())) {
        std::cerr << "Query execution error: " << mysql_error(conn)
                  << std::endl;
        mysql_close(conn);
    }

    mysql_free_result(result);
    mysql_close(conn);
}
std::pair<float, float> getTotalWastedAndUsedOil(std::string currentUserID) {
    char server[26] = "sql11.freesqldatabase.com";
    char username[25] = "sql11647725";
    char password[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        std::cerr << "Unable to connect with MySQL server\n";
        mysql_close(conn);
    }

    std::string selectRestaurantQuery = "SELECT * FROM ORDERS";
    if (mysql_query(conn, selectRestaurantQuery.c_str())) {
        std::cerr << "Query execution error: " << mysql_error(conn)
                  << std::endl;
        mysql_close(conn);
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == nullptr) {
        std::cerr << "Result fetching error." << std::endl;
        mysql_close(conn);
    }

    float totalWastedOil = 0, totalUsedOil = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < mysql_num_fields(result); ++i) {
            if (row[0] == currentUserID) {
                totalWastedOil += std::stoi(row[4]) * std::stoi(row[5]);
                switch (std::stoi(row[6])) {
                    case 0:
                        totalUsedOil += std::stoi(row[4]);
                        break;
                    case 1:
                        totalUsedOil +=
                            std::stoi(row[4]) - (5 / std::stoi(row[4])) * 100;
                        break;
                    case 2:
                        totalUsedOil +=
                            std::stoi(row[4]) - (10 / std::stoi(row[4])) * 100;
                        break;
                    case 3:
                        totalUsedOil += std::stoi(row[4]) -
                                        ((30 / std::stoi(row[4])) * 100);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return std::make_pair(totalWastedOil, totalUsedOil);
}