#include "Menu.h"

#include "../Order/Order.h"
#include "../Profile/Profile.h"
#include "../globalStates/GlobalStates.h"

Order order;
Profile profile;
int Menu::getTerminalWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

void Menu::centeredText(const std::string &text) {
    int terminalWidth = getTerminalWidth();
    int textWidth = text.length();
    int padding = (terminalWidth - textWidth) / 2;

    std::cout << std::setw(padding + textWidth) << text << std::endl;
}
void Menu::header() {
    system("clear");
    centeredText("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    centeredText("\033[32m FOOD MANAGEMENT\n");
};

Menu::Menu() {}

std::pair<MYSQL_RES *, int> Menu::fetchUser(std::string currentUserID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        system("clear");
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    if (mysql_query(conn, "SELECT * FROM RESTAURANTS")) {
        system("clear");
        centeredText("\033[31m Query execution error.");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == nullptr) {
        system("clear");
        centeredText("\033[31m Result fetching error.");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    return std::make_pair(result, 0);
}

void Menu::addMenu(std::string currentUserID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        system("clear");
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return;
    }

    std::string Select_column_query = "SELECT ID FROM MENU";
    if (mysql_query(conn, Select_column_query.c_str())) {
        system("clear");
        centeredText("\033[31m Query execution error.");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *Result = mysql_store_result(conn);

    if (Result == nullptr) {
        system("clear");
        centeredText("\033[31m Result fetching error.");
        mysql_close(conn);
        return;
    }

    std::string Dish_name;
    int Oil_count, Frying_level;
    int Is_menu_added = -1;
    system("clear");
    header();
    centeredText("Now you need to enter in order");
    centeredText("1. The name of the food");
    centeredText("2. The amount of oil used in cooking that food");
    centeredText("3. The frying level (there are 3 choices)");
    centeredText("If you want to finish, just type 'NO'");

    while (true) {
        header();
        centeredText("Enter the name of the food: ");
        std::cin.ignore();
        std::getline(std::cin, Dish_name);
        if (Dish_name == "no" || Dish_name == "No" || Dish_name == "nO" ||
            Dish_name == "NO") {
            ++Is_menu_added;
            break;
        }
        header();
        centeredText("Enter the oil count: ");
        std::cin >> Oil_count;
        while (true) {
            header();
            centeredText("Enter your choice");
            centeredText("0. Zero level (0%)");
            centeredText("1. First level (5%)");
            centeredText("2. Second level (10%)");
            centeredText("3. Third level (15%)");

            std::cin >> Frying_level;
            if (Frying_level != 0 && Frying_level != 1 && Frying_level != 2 &&
                Frying_level != 3) {
                header();
                centeredText("\033[33m Please choose one of the 3 options!!!");
                sleep(2);
                continue;
            }
            break;
        }

        std::string Insert_query =
            "INSERT INTO MENU(ID, DishName, OilCount, FryingLevel) VALUES ('" +
            currentUserID + "','" + Dish_name + "','" +
            std::to_string(Oil_count) + "','" + std::to_string(Frying_level) +
            "')";

        if (mysql_query(conn, Insert_query.c_str())) {
            mysql_close(conn);
            return;
        }
        header();
        centeredText("This food has been added to your menu.");
        centeredText("If you want to finish, just type 'NO'");
    }
    mysql_close(conn);
    if (Is_menu_added != 0) {
        header();
        centeredText("Thank you, your menu has been added successfully!");
    } else {
        header();
        centeredText("You have not added a menu!");
    }
    firstPage();
}

void Menu::showMenu(std::string currentUserID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        system("clear");
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return;
    }

    std::string selectRowQuery =
        "SELECT * FROM MENU WHERE ID = '" + currentUserID + "'";

    if (mysql_query(conn, selectRowQuery.c_str())) {
        system("clear");
        centeredText("\033[31m Query execution error: " +
                     std::string(mysql_error(conn)));
        mysql_close(conn);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == nullptr) {
        system("clear");
        centeredText("\033[31m Result fetching error.");
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

void Menu::deleteMenu(std::string currentUserID) {
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8646145";
    char password[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";
    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        system("clear");
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return;
    }

    std::string Select_column_query = "SELECT ID FROM MENU";
    if (mysql_query(conn, Select_column_query.c_str())) {
        system("clear");
        centeredText("\033[31m Query execution error.");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *Result = mysql_store_result(conn);

    if (Result == nullptr) {
        system("clear");
        centeredText("\033[31m Result fetching error.");
        mysql_close(conn);
        return;
    }

    MYSQL_ROW Row;
    bool Check_menu_ID = false;
    while (Row = mysql_fetch_row(Result)) {
        if (Row[0] != nullptr && Row[0] == currentUserID) {
            Check_menu_ID = true;
            std::string Delete_row_query =
                "DELETE FROM MENU WHERE ID = '" + currentUserID + "'";
            if (mysql_query(conn, Delete_row_query.c_str())) {
                system("clear");
                centeredText("\033[31m Query execution error: " +
                             std::string(mysql_error(conn)));
                mysql_close(conn);
                return;
            }
        }
    }

    mysql_close(conn);

    if (!Check_menu_ID) {
        system("clear");
        centeredText("\033[31m Sorry, you didn't add a menu!");
        firstPage();
    } else {
        header();
        centeredText("Your menu has been deleted successfully!");
        firstPage();
    }
}

void Menu::firstPage() {
    std::string currentUserID = GlobalStates::currentUserID;
    int answer;
    Menu obj;
    std::cout << "\t * Press 1 to Add Menu    " << std::endl;
    std::cout << "\t * Press 2 to Edit Menu   " << std::endl;
    std::cout << "\t * Press 3 to Show Menu   " << std::endl;
    std::cout << "\t * Press 4 to Delete Menu " << std::endl;
    std::cout << "\t * Press 5 to Go to Profile" << std::endl;
    std::cout << "\t * Press 6 to EXIT \n\n";
    header();
    centeredText(" * Press 1 to Add Menu");
    centeredText(" * Press 2 to Edit Menu");
    centeredText(" * Press 3 to Show Menu");
    centeredText(" * Press 4 to Delete Menu");
    centeredText(" * Press 5 to Take Order");
    centeredText(" * Press 6 to EXIT");

    std::cout << "\t Enter your choice: ";

    std::cin >> answer;
    switch (answer) {
        case 1:
            obj.addMenu(currentUserID);
            break;
        case 2:
            editMenu(currentUserID);
            break;
        case 3:
            obj.showMenu(currentUserID);
            break;
        case 4:
            obj.deleteMenu(currentUserID);
            break;
        case 5:
            profile.showProfilePage();
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

void Menu::editMenu(std::string currentUserID) {
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
    std::string Select_column_query =
        "SELECT * FROM MENU WHERE ID = '" + currentUserID + "'";
    if (mysql_query(conn, Select_column_query.c_str())) {
        std::cerr << "Query execution error." << std::endl;
        mysql_close(conn);
        return;
    }
    MYSQL_RES *firstResult = mysql_store_result(conn);

    if (firstResult == nullptr) {
        std::cerr << "Result fetching error." << std::endl;
        mysql_close(conn);
        return;
    }
    Menu Restaurant;
    int editDelete, secondChoise, newOilCount, newFryingLevel, Rows_count = 0,
                                                               Row_index = 0;
    std::string oldDishName, newDishName, oldOilCount, oldFryingLevel,
        updateRowQuery, firstChoise;
    MYSQL_ROW firstRow;

    while (firstRow = mysql_fetch_row(firstResult)) {
        Rows_count++;
    }
    mysql_free_result(firstResult);
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
    std::cout << "1. Edit the some food\n2. Delete the some food\n3. Back\n";
    while (editDelete != 1 && editDelete != 2 && editDelete != 3) {
        std::cout << "Please enter the number from 1 to 3 2\n";
        std::cout << "Answer: ";
        std::cin >> editDelete;
    }

    if (editDelete == 1) {
        std::cout << "\nIts your menu, choose what position you want to "
                     "edit!!!\nFor cancel enter x.\n";
        Restaurant.showMenu(currentUserID);
        while (true) {
            std::cout << "Answer: ";
            std::cin >> firstChoise;
            if (firstChoise == "x" || firstChoise == "X") {
                break;
            } else {
                std::stringstream convertString(firstChoise);
                int number;
                if (!(convertString >> number)) {
                    std::cerr << "Invalid input: Not a valid integer."
                              << std::endl;  // edit
                    return;
                }
                if (number >= 1 && number <= Rows_count) {
                    while ((Row = mysql_fetch_row(Result))) {
                        Row_index++;
                        if (Row_index == number) {
                            oldDishName = Row[1];
                            oldOilCount = Row[2];
                            oldFryingLevel = Row[3];
                        }
                    }
                    std::cout << "Which position do you want to change, enter "
                                 "the corresponding number.\n";
                    std::cout << "1. Dishname\n2. Oil count\n3. Frying level\n";
                    while (secondChoise != 1 && secondChoise != 2 &&
                           secondChoise != 3) {
                        std::cout << "Please enter the number from 1 to 3\n";
                        std::cout << "Answer: ";
                        std::cin >> secondChoise;
                    }
                    switch (secondChoise) {
                        case 1:
                            std::cout << "New Dishname: ";
                            std::cin.ignore();
                            std::getline(std::cin, newDishName);
                            updateRowQuery =
                                "UPDATE MENU SET DishName = '" + newDishName +
                                "' WHERE ID = '" + currentUserID +
                                "' AND DishName = '" + oldDishName +
                                "' AND OilCount = " + oldOilCount +
                                " AND FryingLevel = " + oldFryingLevel;
                            if (mysql_query(conn, updateRowQuery.c_str())) {
                                std::cerr << "Query execution error: "
                                          << mysql_error(conn) << std::endl;
                                mysql_close(conn);
                                return;
                            }
                            break;
                        case 2:
                            std::cout << "New Oil Count: ";
                            std::cin >> newOilCount;
                            updateRowQuery =
                                "UPDATE MENU SET OilCount = " +
                                std::to_string(newOilCount) + " WHERE ID = '" +
                                currentUserID + "' AND DishName = '" +
                                oldDishName +
                                "' AND OilCount = " + oldOilCount +
                                " AND FryingLevel = " + oldFryingLevel;
                            if (mysql_query(conn, updateRowQuery.c_str())) {
                                std::cerr << "Query execution error: "
                                          << mysql_error(conn) << std::endl;
                                mysql_close(conn);
                                return;
                            }
                            break;
                        case 3:
                            std::cout
                                << "New Frying Level(there are 3 choises)\n";
                            while (true) {
                                std::cout << "Enter your choise\n";
                                std::cout << "0. Zero level(0%)\n";
                                std::cout << "1. First level(5%)\n";
                                std::cout << "2. Second level(10%)\n";
                                std::cout << "3. Third level(15%)\n";
                                std::cin >> newFryingLevel;
                                if (newFryingLevel != 0 &&
                                    newFryingLevel != 1 &&
                                    newFryingLevel != 2 &&
                                    newFryingLevel != 3) {
                                    std::cout << "\tPlease choose one of the 3 "
                                                 "options!!!\n";
                                    continue;
                                }
                                break;
                            }
                            updateRowQuery =
                                "UPDATE MENU SET FryingLevel = " +
                                std::to_string(newFryingLevel) +
                                " WHERE ID = '" + currentUserID +
                                "' AND DishName = '" + oldDishName +
                                "' AND OilCount = " + oldOilCount +
                                " AND FryingLevel = " + oldFryingLevel;
                            if (mysql_query(conn, updateRowQuery.c_str())) {
                                std::cerr << "Query execution error: "
                                          << mysql_error(conn) << std::endl;
                                mysql_close(conn);
                                return;
                            }
                            break;
                        default:
                            break;
                    }
                    std::cout
                        << "Do you want to change any position again?\nIf yes, "
                           "enter which position, otherwise x!!!\n";
                    continue;
                } else {
                    std::cout << "Please enter the number from 1 to "
                              << Rows_count << "!!!\n";
                    continue;
                }
            }
        }
    } else if (editDelete == 2) {
        std::cout << "\nIts your menu, choose what position you want to "
                     "delete!!!\nFor cancel enter x.\n";
        Restaurant.showMenu(currentUserID);
        while (true) {
            std::cout << "Answer: ";
            std::cin >> firstChoise;
            if (firstChoise == "x" || firstChoise == "X") {
                break;
            } else {
                std::stringstream convertString(firstChoise);
                int number;
                if (!(convertString >> number)) {
                    std::cerr << "Invalid input: Not a valid integer."
                              << std::endl;  // edit
                    return;
                }
                if (number >= 1 && number <= Rows_count) {
                    while ((Row = mysql_fetch_row(Result))) {
                        Row_index++;
                        if (Row_index == number) {
                            oldDishName = Row[1];
                            oldOilCount = Row[2];
                            oldFryingLevel = Row[3];
                        }
                    }
                    std::string deleteRowQuery =
                        "DELETE FROM MENU WHERE ID = '" + currentUserID +
                        "' AND DishName = '" + oldDishName +
                        "' AND OilCount = " + oldOilCount +
                        " AND FryingLevel = " + oldFryingLevel;
                    if (mysql_query(conn, deleteRowQuery.c_str())) {
                        std::cerr
                            << "Query execution error: " << mysql_error(conn)
                            << std::endl;
                        mysql_close(conn);
                        return;
                    }
                    std::cout << "The dish " << oldDishName
                              << " was deleted successfully!!!\n";
                    std::cout
                        << "Do you want to delete any position again?\nIf yes, "
                           "enter which position, otherwise x!!!\n";
                    continue;
                } else {
                    std::cout << "Please enter the number from 1 to "
                              << Rows_count << "!!!\n";
                    continue;
                }
            }
        }
    }

    mysql_close(conn);
}