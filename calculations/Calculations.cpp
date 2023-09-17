#include "./Calculations.h"

float calculateFrequencyOnOrder(bool post, std::string restID,
                                std::string currentUserID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    std::pair<float, float> totalWastedAndUsedOil =
        getTotalWastedAndUsedOil(restID);

    float totalWastedOil = totalWastedAndUsedOil.first / 7;
    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        std::cerr << "Unable to connect with MySQL server\n";
        mysql_close(conn);
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

    float maxOilCount = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        maxOilCount = std::stoi(row[7]);
    }
    float frequency = totalWastedOil / maxOilCount;
    if (post) {
        std::string updateRowQuery =
            "UPDATE RESTAURANTS SET Frequency = " + std::to_string(frequency) +
            " WHERE ID = '" + currentUserID + "'";

        if (mysql_query(conn, updateRowQuery.c_str())) {
            std::cerr << "Query execution error: " << mysql_error(conn)
                      << std::endl;
            mysql_close(conn);
        }
    }

    mysql_free_result(result);
    mysql_close(conn);
    return frequency;
}

float calculateCoefficientOnOrder(bool post, std::string currentUserID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

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
    std::string restID = "Tashir_ID";
    std::pair<float, float> totalWastedAndUsedOil =
        getTotalWastedAndUsedOil(restID);
    float totalWastedOil = totalWastedAndUsedOil.first;
    float totalUsedOil = totalWastedAndUsedOil.second;
    float coefficient = (totalUsedOil / 7) / (totalWastedOil / 7);

    std::string currID = "2196617506373257558";
    std::string stringCoefficient = std::to_string(coefficient);
    if (post) {
        std::string updateRowQuery =
            "UPDATE RESTAURANTS SET Coefficient = " + stringCoefficient +
            " WHERE ID = '" + currID + "'";

        if (mysql_query(conn, updateRowQuery.c_str())) {
            std::cerr << "Query execution error: " << mysql_error(conn)
                      << std::endl;
            mysql_close(conn);
        }
    }
    calculateFrequencyOnOrder(true, restID, currID);

    mysql_free_result(result);
    mysql_close(conn);
    return coefficient;
}
