#include "Menu.h"

#include "../Authentication/Authentication.h"
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
    std::cout << std::setw(padding + textWidth)
              << "\033[1;40m" + text + "\033[0m\n"
              << std::endl;
}
void Menu::header() {
    centeredText("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    centeredText("FOOD MANAGEMENT");
};

Menu::Menu() {}

std::pair<MYSQL_RES *, int> Menu::fetchUser(std::string currentUserID) {
    char server[26] = "sql11.freesqldatabase.com";
    char username[25] = "sql11647725";
    char password[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    if (mysql_query(conn, "SELECT * FROM RESTAURANTS")) {
        centeredText("\033[31m Query execution error.");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == nullptr) {
        centeredText("\033[31m Result fetching error.");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    return std::make_pair(result, 0);
}

void Menu::addMenu(std::string currentUserID) {
    system("clear");
    char server[26] = "sql11.freesqldatabase.com";
    char username[25] = "sql11647725";
    char password[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return;
    }
    std::string Select_column_query = "SELECT ID FROM MENU";
    if (mysql_query(conn, Select_column_query.c_str())) {
        centeredText("\033[31m Query execution error.");
        mysql_close(conn);
        return;
    }
    MYSQL_RES *Result = mysql_store_result(conn);
    if (Result == nullptr) {
        centeredText("\033[31m Result fetching error.");
        mysql_close(conn);
        return;
    }
    std::string Dish_name;
    int Oil_count, Frying_level;
    int Is_menu_added = -1;
    Table addMenuTableHeading;
    Table addMenuTable;

    addMenuTableHeading.add_row({"Please fill following details."});
    addMenuTableHeading.format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << addMenuTableHeading << std::endl;

    addMenuTable.add_row({"1. The name of Dish",
                          "2. The amount of Oil used to cook the Dish",
                          "3. The frying level (there are 4 choices)",
                          "4. If you want to finish, just type 'NO'"});
    addMenuTable[0]
        .format()
        .font_color(Color::yellow)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .width(30);
    std::cout << addMenuTable << std::endl;
    addMenuTable.add_row({"Enter Your Choice: "});
    addMenuTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);

    while (true) {
        header();
        Table foodNameTable;
        foodNameTable.add_row({"Enter the name of the food:"});
        foodNameTable.format()
            .background_color(Color::green)
            .font_align(FontAlign::center);
        std::cout << foodNameTable << "\t";
        std::cin.ignore();
        std::getline(std::cin, Dish_name);
        std::cout << "\n";

        if (Dish_name == "no" || Dish_name == "No" || Dish_name == "nO" ||
            Dish_name == "NO") {
            system("clear");
            ++Is_menu_added;
            break;
        }

        while (true) {
            Table oilCountTable;
            oilCountTable.add_row({"Enter the oil count: "});
            oilCountTable.format()
                .background_color(Color::green)
                .font_align(FontAlign::center);
            std::cout << oilCountTable << "\t";
            std::cin >> Oil_count;
            std::cout << "\n";

            Table fryingLevelTable;
            fryingLevelTable.add_row({"Enter the oil count: "});
            fryingLevelTable.format()
                .background_color(Color::green)
                .font_align(FontAlign::center);
            std::cout << fryingLevelTable << std::endl;

            Table levelsTable;
            levelsTable.add_row({"1. Zero level (0%)", "2. First level (5%)",
                                 "3. Second level (10%)",
                                 "4. Third level (15%)"});
            levelsTable[0]
                .format()
                .font_color(Color::yellow)
                .font_align(FontAlign::center)
                .font_style({FontStyle::bold})
                .width(30);
            std::cout << levelsTable << std::endl;
            std::cin >> Frying_level;

            if (Frying_level != 4 && Frying_level != 1 && Frying_level != 2 &&
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
        system("clear");
        centeredText("This food has been added to your menu.");
        centeredText("If you want to finish, just type 'NO'");
    }
    mysql_close(conn);
    if (Is_menu_added != 0) {
        system("clear");
        header();
        centeredText("Thank you, your menu has been added successfully!");
    } else {
        header();
        system("clear");
        centeredText("You have not added a menu!");
    }
    system("clear");

    firstPage();
}
void Menu::showMenu(std::string currentUserID) {
    char server[26] = "sql11.freesqldatabase.com";
    char username[25] = "sql11647725";
    char password[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return;
    }

    std::string selectRowQuery =
        "SELECT * FROM MENU WHERE ID = '" + currentUserID + "'";

    if (mysql_query(conn, selectRowQuery.c_str())) {
        centeredText("\033[31m Query execution error: " +
                     std::string(mysql_error(conn)));
        mysql_close(conn);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == nullptr) {
        centeredText("\033[31m Result fetching error.");
        mysql_close(conn);
        return;
    }

    MYSQL_ROW row;
    std::cout << "\t\t\t\t\t+---------------+-------------+--------------+"
              << std::endl;
    std::cout << "\t\t\t\t\t|DishName       | OilCount    |Frying  Level |"
              << std::endl;
    std::cout << "\t\t\t\t\t+---------------+-------------+--------------+"
              << std::endl;
    Authentication auth;
    while ((row = mysql_fetch_row(result))) {
        for (int i = 1; i < mysql_num_fields(result); ++i) {
            if (row[i] != nullptr) {
                if (i % 3 == 1) {
                    std::cout << "\t\t\t\t\t|";
                    std::size_t length = std::strlen(row[i]);
                    std::cout << row[i];
                    auth.space(14 - length);
                    std::cout << "|";
                } else if (i % 3 == 2) {
                    std::size_t length = std::strlen(row[i]);
                    std::cout << row[i];
                    auth.space(12 - length);
                    std::cout << "|";
                }
                if (i % 3 == 0) {
                    std::size_t length = std::strlen(row[i]);
                    std::cout << row[i];
                    auth.space(13 - length);
                    std::cout << "|";
                    std::cout << std::endl;
                    std::cout << "\t\t\t\t\t+---------------+-------------+----"
                                 "----------+"
                              << std::endl;
                }
            } else {
                std::cout << "NULL ";
            }
        }
    }
    firstPage();

    mysql_free_result(result);
    mysql_close(conn);
}

void Menu::deleteMenu(std::string currentUserID) {
    char server[26] = "sql11.freesqldatabase.com";
    char username[25] = "sql11647725";
    char password[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return;
    }

    std::string Select_column_query = "SELECT ID FROM MENU";
    if (mysql_query(conn, Select_column_query.c_str())) {
        centeredText("\033[31m Query execution error.");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *Result = mysql_store_result(conn);

    if (Result == nullptr) {
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
                centeredText("\033[31m Query execution error: " +
                             std::string(mysql_error(conn)));
                mysql_close(conn);
                return;
            }
        }
    }

    mysql_close(conn);

    if (!Check_menu_ID) {
        centeredText("\033[31m Sorry, you didn't add a menu!");
        firstPage();
    } else {
        header();
        centeredText("Your menu has been deleted successfully!");
        firstPage();
    }
}

void Menu::firstPage() {
    Table firstPageTable;
    std::string currentUserID = GlobalStates::currentUserID;
    int answer;
    Menu obj;
    header();

    firstPageTable.add_row({"1. Add Menu", "2. Edit Menu", "3. Show Menu",
                            "4. Delete Menu", "5. Profile", "6. Exit"});
    firstPageTable[0]
        .format()
        .font_color(Color::yellow)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .width(30);
    std::cout << firstPageTable << std::endl;
    firstPageTable.add_row({"Enter Your Choice: "});
    firstPageTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << "\t";
    std::cin >> answer;
    std::cout << std::endl;

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
            centeredText("Thank You!");
            break;
        default:
            std::cout << "\033[2J\033[1;1H";
            std::cout << "\t\t\t Select from the options given above \n";
            firstPage();
    }
}

void Menu::editMenu(std::string currentUserID) {
    char server[26] = "sql11.freesqldatabase.com";
    char username[25] = "sql11647725";
    char password[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr) {
        centeredText("Unable to connect with MySQL server");
        mysql_close(conn);
        return;
    }

    std::string Select_column_query =
        "SELECT * FROM MENU WHERE ID = '" + currentUserID + "'";

    if (mysql_query(conn, Select_column_query.c_str())) {
        centeredText("Query execution error.");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *firstResult = mysql_store_result(conn);

    if (firstResult == nullptr) {
        centeredText("Result fetching error.");
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
        centeredText("Query execution error.");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *Result = mysql_store_result(conn);

    if (Result == nullptr) {
        centeredText("Result fetching error.");
        mysql_close(conn);
        return;
    }

    MYSQL_ROW Row;
    header();
    centeredText("1. Edit the some food");
    centeredText("2. Delete the some food");
    centeredText("3. Back");

    while (editDelete != 1 && editDelete != 2 && editDelete != 3) {
        centeredText("Please enter the number from 1 to 3");
        std::cout << ("Answer: ");
        std::cin >> editDelete;
    }
    header();
    if (editDelete == 1) {
        centeredText(
            "It's your menu, choose what position you want to edit!!!");
        centeredText("For cancel enter x.");

        Restaurant.showMenu(currentUserID);

        while (true) {
            std::cout << ("Answer: ");
            std::cin >> firstChoise;

            if (firstChoise == "x" || firstChoise == "X") {
                break;
            } else {
                std::stringstream convertString(firstChoise);
                int number;

                if (!(convertString >> number)) {
                    centeredText("Invalid input: Not a valid integer.");
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

                    centeredText(
                        "Which position do you want to change, enter the "
                        "corresponding number.");
                    centeredText("1. Dishname");
                    centeredText("2. Oil count");
                    centeredText("3. Frying level");

                    while (secondChoise != 1 && secondChoise != 2 &&
                           secondChoise != 3) {
                        centeredText("Please enter the number from 1 to 3");
                        std::cout << ("Answer: ");
                        std::cin >> secondChoise;
                    }
                    header();
                    switch (secondChoise) {
                        case 1:
                            centeredText("New Dishname: ");
                            std::cin.ignore();
                            std::getline(std::cin, newDishName);

                            updateRowQuery =
                                "UPDATE MENU SET DishName = '" + newDishName +
                                "' WHERE ID = '" + currentUserID +
                                "' AND DishName = '" + oldDishName +
                                "' AND OilCount = " + oldOilCount +
                                " AND FryingLevel = " + oldFryingLevel;

                            if (mysql_query(conn, updateRowQuery.c_str())) {
                                centeredText("Query execution error: " +
                                             std::string(mysql_error(conn)));
                                mysql_close(conn);
                                return;
                            }

                            break;

                        case 2:
                            centeredText("New Oil Count: ");
                            std::cin >> newOilCount;

                            updateRowQuery =
                                "UPDATE MENU SET OilCount = " +
                                std::to_string(newOilCount) + " WHERE ID = '" +
                                currentUserID + "' AND DishName = '" +
                                oldDishName +
                                "' AND OilCount = " + oldOilCount +
                                " AND FryingLevel = " + oldFryingLevel;

                            if (mysql_query(conn, updateRowQuery.c_str())) {
                                centeredText("Query execution error: " +
                                             std::string(mysql_error(conn)));
                                mysql_close(conn);
                                return;
                            }

                            break;

                        case 3:
                            centeredText(
                                "New Frying Level (there are 3 choices)");

                            while (true) {
                                centeredText("Enter your choice");
                                centeredText("0. Zero level (0%)");
                                centeredText("1. First level (5%)");
                                centeredText("2. Second level (10%)");
                                centeredText("3. Third level (15%)");
                                std::cin >> newFryingLevel;

                                if (newFryingLevel != 0 &&
                                    newFryingLevel != 1 &&
                                    newFryingLevel != 2 &&
                                    newFryingLevel != 3) {
                                    centeredText(
                                        "Please choose one of the 3 "
                                        "options!!!");
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
                                centeredText("Query execution error: " +
                                             std::string(mysql_error(conn)));
                                mysql_close(conn);
                                return;
                            }

                            break;

                        default:
                            break;
                    }

                    centeredText("Do you want to change any position again?");
                    centeredText(
                        "If yes, enter which position, otherwise x!!!");
                    continue;
                } else {
                    centeredText("Please enter the number from 1 to " +
                                 std::to_string(Rows_count) + "!!!");
                    continue;
                }
            }
        }
    } else if (editDelete == 2) {
        centeredText(
            "It's your menu, choose what position you want to delete!!!");
        centeredText("For cancel enter x.");

        Restaurant.showMenu(currentUserID);

        while (true) {
            std::cout << ("Answer: ");
            std::cin >> firstChoise;

            if (firstChoise == "x" || firstChoise == "X") {
                break;
            } else {
                std::stringstream convertString(firstChoise);
                int number;

                if (!(convertString >> number)) {
                    centeredText("Invalid input: Not a valid integer.");
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
                        centeredText("Query execution error: " +
                                     std::string(mysql_error(conn)));
                        mysql_close(conn);
                        return;
                    }

                    centeredText("The dish " + oldDishName +
                                 " was deleted successfully!!!");
                    centeredText("Do you want to delete any position again?");
                    centeredText(
                        "If yes, enter which position, otherwise x!!!");
                    continue;
                } else {
                    centeredText("Please enter the number from 1 to " +
                                 std::to_string(Rows_count) + "!!!");
                    continue;
                }
            }
        }
    }
    firstPage();

    mysql_close(conn);
}