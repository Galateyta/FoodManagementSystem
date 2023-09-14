#include "./Calculations.h"

void calculateFrequencyOnOrder(std::string currentUserID,
                               float totalWastedOil) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        std::cerr << "Unable to connect with MySQL server\n";
        mysql_close(conn);
        return;
    }

    std::string selectRowQuery =
        "SELECT * FROM RESTAURANTS WHERE ID = '" + currentUserID + "'";

    if (mysql_query(conn, selectRowQuery.c_str())) {
        std::cerr << "Query execution error: " << mysql_error(conn)
                  << std::endl;
        mysql_close(conn);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == nullptr) {
        std::cerr << "Result fetching error." << std::endl;
        mysql_close(conn);
        return;
    }

    float maxOilCount = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        maxOilCount = std::stoi(row[7]);
    }
    float frequency = totalWastedOil / maxOilCount;

    std::string updateRowQuery =
        "UPDATE RESTAURANTS SET Frequency = " + std::to_string(frequency) +
        " WHERE ID = '" + currentUserID + "'";

    if (mysql_query(conn, updateRowQuery.c_str())) {
        std::cerr << "Query execution error: " << mysql_error(conn)
                  << std::endl;
        mysql_close(conn);
        return;
    }
    mysql_free_result(result);
    mysql_close(conn);
}

void calculateCoefficientOnOrder(std::string currentUserID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        std::cerr << "Unable to connect with MySQL server\n";
        mysql_close(conn);
        return;
    }

    std::string selectRestaurantQuery = "SELECT * FROM ORDERS";
    if (mysql_query(conn, selectRestaurantQuery.c_str())) {
        std::cerr << "Query execution error: " << mysql_error(conn)
                  << std::endl;
        mysql_close(conn);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == nullptr) {
        std::cerr << "Result fetching error." << std::endl;
        mysql_close(conn);
        return;
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

    float coefficient = (totalUsedOil / 7) / (totalWastedOil / 7);
    std::string currID = "2196617506373257558";
    std::string stringCoefficient = std::to_string(coefficient);
    std::string updateRowQuery =
        "UPDATE RESTAURANTS SET Coefficient = " + stringCoefficient +
        " WHERE ID = '" + currID + "'";

    if (mysql_query(conn, updateRowQuery.c_str())) {
        std::cerr << "Query execution error: " << mysql_error(conn)
                  << std::endl;
        mysql_close(conn);
        return;
    }
    calculateFrequencyOnOrder(currID, totalWastedOil / 7);
    mysql_free_result(result);
    mysql_close(conn);
}
