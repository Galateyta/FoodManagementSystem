#include "Menu.h"

#include "../Order/Order.h"

Order order;

Menu::Menu(){};
std::pair<MYSQL_RES *, int> Menu::fetchUser(std::string Current_user_ID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8644761";
    char password[15] = "M8c6DWvEMr";
    char database[15] = "sql8644761";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        std::cerr << "Unable to connect with MySQL server\n";
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    if (mysql_query(conn, "SELECT * FROM RESTAURANTS")) {
        std::cerr << "Query execution error." << std::endl;
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == nullptr) {
        std::cerr << "Result fetching error." << std::endl;
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    return std::make_pair(result, 0);
}

void Menu::addMenu(std::string Current_user_ID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8644761";
    char password[15] = "M8c6DWvEMr";
    char database[15] = "sql8644761";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        std::cerr << "Unable to connect with MySQL server\n";
        mysql_close(conn);
        return;
    }

    std::string Select_column_query = "SELECT ID FROM MENU";
    if (mysql_query(conn, Select_column_query.c_str())) {
        std::cerr << "Query execution error." << std::endl;
        mysql_close(conn);
        return;
    }

    MYSQL_RES *Result = mysql_store_result(conn);

    if (Result == nullptr) {
        std::cerr << "Result fetching error." << std::endl;
        mysql_close(conn);
        return;
    }

    std::string Dish_name;
    int Oil_count, Frying_level;
    int Is_menu_added = -1;

    std::cout << "\n\tNow you need to enter in order\n";
    std::cout << "1. The name of the food\n";
    std::cout << "2. The amount of oil used in cooking that food\n";
    std::cout << "3. The frying level(there are 3 choices)\n\n";
    std::cout << "\tIf you want to finish, just type 'NO'\n\n";
    while (true) {
        std::cout << "Enter the name of the food: ";
        std::cin.ignore();
        std::getline(std::cin, Dish_name);
        if (Dish_name == "no" || Dish_name == "No" || Dish_name == "nO" ||
            Dish_name == "NO") {
            ++Is_menu_added;
            break;
        }
        std::cout << "Enter the oil count: ";
        std::cin >> Oil_count;
        while (true) {
            std::cout << "Enter your choice\n";
            std::cout << "0. Zero level(0%)\n";
            std::cout << "1. First level(5%)\n";
            std::cout << "2. Second level(10%)\n";
            std::cout << "3. Third level(15%)\n";  // can be modified
            std::cin >> Frying_level;
            if (Frying_level != 0 && Frying_level != 1 && Frying_level != 2 &&
                Frying_level != 3) {
                std::cout << "\tPlease choose one of the 3 options!!!\n";
                continue;
            }
            break;
        }

        std::string Insert_query =
            "INSERT INTO MENU(ID, DishName, OilCount, FryingLevel) VALUES ('" +
            Current_user_ID + "','" + Dish_name + "','" +
            std::to_string(Oil_count) + "','" + std::to_string(Frying_level) +
            "')";

        if (mysql_query(conn, Insert_query.c_str())) {
            // I dont know why it is like this
            mysql_close(conn);
            return;
        }
        std::cout << "\033[2J\033[1;1H";
        std::cout << "\tThis food has been added to your menu.\n";
        std::cout << "\tIf you want to finish, just type 'NO'\n";
    }
    mysql_close(conn);
    if (Is_menu_added != 0) {
        std::cout << "\n\tThank you, your menu has benn added "
                     "successfully!!!\n\n";  // if no food is added, do not
                                             // write this line
    } else {
        std::cout << "\n\tYou have not added a menu!!!\n\n";
    }
    firstPage();
    return;
}

void Menu::showMenu(std::string Current_user_ID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8644761";
    char password[15] = "M8c6DWvEMr";
    char database[15] = "sql8644761";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        std::cerr << "Unable to connect with MySQL server\n";
        mysql_close(conn);
        return;
    }

    std::string selectRowQuery =
        "SELECT * FROM MENU WHERE ID = '" + Current_user_ID + "'";

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

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        for (int i = 1; i < mysql_num_fields(result); ++i) {
            if (row[i] != nullptr) {
                std::cout << row[i] << " ";
            } else {
                std::cout << "NULL ";
            }
        }
        std::cout << std::endl;
    }

    mysql_free_result(result);
    mysql_close(conn);
}

void Menu::deleteMenu(std::string Current_user_ID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8644761";
    char password[15] = "M8c6DWvEMr";
    char database[15] = "sql8644761";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        std::cerr << "Unable to connect with MySQL server\n";
        mysql_close(conn);
        return;
    }

    std::string Select_column_query = "SELECT ID FROM MENU";
    if (mysql_query(conn, Select_column_query.c_str())) {
        std::cerr << "Query execution error." << std::endl;
        mysql_close(conn);
        return;
    }

    MYSQL_RES *Result = mysql_store_result(conn);

    if (Result == nullptr) {
        std::cerr << "Result fetching error." << std::endl;
        mysql_close(conn);
        return;
    }

    MYSQL_ROW Row;
    bool Check_menu_ID = false;
    while (Row = mysql_fetch_row(Result)) {
        if (Row[0] != nullptr && Row[0] == Current_user_ID) {
            Check_menu_ID = true;
            std::string Delete_row_query =
                "DELETE FROM MENU WHERE ID = '" + Current_user_ID + "'";
            if (mysql_query(conn, Delete_row_query.c_str())) {
                std::cerr << "Query execution error: " << mysql_error(conn)
                          << std::endl;
                mysql_close(conn);
                return;
            }
        }
    }

    mysql_close(conn);

    if (!Check_menu_ID) {
        std::cout << "\n\tSorry, you didn`t add menu!!!\n\n";
        firstPage();
        return;
    } else {
        std::cout << "\n\tYour menu has benn deleted successfully!!!\n\n";
        firstPage();
    }
}

void Menu::firstPage() {
    int answer;
    Menu obj;
    std::cout << "\t * Press 1 to Add Menu    " << std::endl;
    std::cout << "\t * Press 2 to Edit Menu   " << std::endl;
    std::cout << "\t * Press 3 to Show Menu   " << std::endl;
    std::cout << "\t * Press 4 to Delete Menu " << std::endl;
    std::cout << "\t * Press 5 to Take Order" << std::endl;
    std::cout << "\t * Press 6 to EXIT \n\n";
    std::cout << "\t Enter your choice: ";

    std::cin >> answer;
    switch (answer) {
        case 1:
            obj.addMenu("Tashir_ID");
            break;
        case 2:
            // Edit_menu();
            break;
        case 3:
            obj.showMenu("Tashir_ID");
            break;
        case 4:
            obj.deleteMenu("Tashir_ID");
            break;
        case 5:
            order.fillOrderData(
                "Tashir_ID");  // TODO change this to current user id
            break;
        case 6:
            std::cout << "\t\t\t Thank you! \n\n";
            break;
        default:
            std::cout << "\033[2J\033[1;1H";
            std::cout << "\t\t\t Select  from the options given above \n";
            firstPage();
    }
}