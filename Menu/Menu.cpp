#include "Menu.h"
#include "../Order/Order.h"
Order order;

int Menu::getTerminalWidth()
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

void Menu::centeredText(const std::string &text)
{
    int terminalWidth = getTerminalWidth();
    int textWidth = text.length();
    int padding = (terminalWidth - textWidth) / 2;

    std::cout << std::setw(padding + textWidth) << text << std::endl;
}
void Menu::header()
{
    system("clear");
    centeredText("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    centeredText("\033[32m FOOD MANAGEMENT\n");
};

Menu::Menu() {}

std::pair<MYSQL_RES *, int> Menu::fetchUser(std::string Current_user_ID)
{
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8644761";
    char password[15] = "M8c6DWvEMr";
    char database[15] = "sql8644761";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr)
    {
        system("clear");
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    if (mysql_query(conn, "SELECT * FROM RESTAURANTS"))
    {
        system("clear");
        centeredText("\033[31m Query execution error.");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == nullptr)
    {
        system("clear");
        centeredText("\033[31m Result fetching error.");
        mysql_close(conn);
        return std::make_pair(nullptr, 1);
    }

    return std::make_pair(result, 0);
}

void Menu::addMenu(std::string Current_user_ID)
{
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8644761";
    char password[15] = "M8c6DWvEMr";
    char database[15] = "sql8644761";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr)
    {
        system("clear");
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return;
    }

    std::string Select_column_query = "SELECT ID FROM MENU";
    if (mysql_query(conn, Select_column_query.c_str()))
    {
        system("clear");
        centeredText("\033[31m Query execution error.");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *Result = mysql_store_result(conn);

    if (Result == nullptr)
    {
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

    while (true)
    {
        header();
        centeredText("Enter the name of the food: ");
        std::cin.ignore();
        std::getline(std::cin, Dish_name);
        if (Dish_name == "no" || Dish_name == "No" || Dish_name == "nO" ||
            Dish_name == "NO")
        {
            ++Is_menu_added;
            break;
        }
        header();
        centeredText("Enter the oil count: ");
        std::cin >> Oil_count;
        while (true)
        {
            header();
            centeredText("Enter your choice");
            centeredText("0. Zero level (0%)");
            centeredText("1. First level (5%)");
            centeredText("2. Second level (10%)");
            centeredText("3. Third level (15%)");

            std::cin >> Frying_level;
            if (Frying_level != 0 && Frying_level != 1 && Frying_level != 2 &&
                Frying_level != 3)
            {
                header();
                centeredText("\033[33m Please choose one of the 3 options!!!");
                sleep(2);
                continue;
            }
            break;
        }

        std::string Insert_query =
            "INSERT INTO MENU(ID, DishName, OilCount, FryingLevel) VALUES ('" +
            Current_user_ID + "','" + Dish_name + "','" +
            std::to_string(Oil_count) + "','" + std::to_string(Frying_level) +
            "')";

        if (mysql_query(conn, Insert_query.c_str()))
        {
            mysql_close(conn);
            return;
        }
        header();
        centeredText("This food has been added to your menu.");
        centeredText("If you want to finish, just type 'NO'");
    }
    mysql_close(conn);
    if (Is_menu_added != 0)
    {
        header();
        centeredText("Thank you, your menu has been added successfully!");
    }
    else
    {
        header();
        centeredText("You have not added a menu!");
    }
    firstPage();
}

void Menu::showMenu(std::string Current_user_ID)
{
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8644761";
    char password[15] = "M8c6DWvEMr";
    char database[15] = "sql8644761";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr)
    {
        system("clear");
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return;
    }

    std::string selectRowQuery =
        "SELECT * FROM MENU WHERE ID = '" + Current_user_ID + "'";

    if (mysql_query(conn, selectRowQuery.c_str()))
    {
        system("clear");
        centeredText("\033[31m Query execution error: " + std::string(mysql_error(conn)));
        mysql_close(conn);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == nullptr)
    {
        system("clear");
        centeredText("\033[31m Result fetching error.");
        mysql_close(conn);
        return;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 1; i < mysql_num_fields(result); ++i)
        {
            if (row[i] != nullptr)
            {
                std::cout << row[i] << " ";
            }
            else
            {
                std::cout << "NULL ";
            }
        }
        std::cout << std::endl;
    }

    mysql_free_result(result);
    mysql_close(conn);
}

void Menu::deleteMenu(std::string Current_user_ID)
{
    char server[26] = "sql8.freesqldatabase.com";
    char username[15] = "sql8644761";
    char password[15] = "M8c6DWvEMr";
    char database[15] = "sql8644761";

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, server, username, password, database, 0,
                           nullptr, 0) == nullptr)
    {
        system("clear");
        centeredText("\033[31m Unable to connect with MySQL server");
        mysql_close(conn);
        return;
    }

    std::string Select_column_query = "SELECT ID FROM MENU";
    if (mysql_query(conn, Select_column_query.c_str()))
    {
        system("clear");
        centeredText("\033[31m Query execution error.");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *Result = mysql_store_result(conn);

    if (Result == nullptr)
    {
        system("clear");
        centeredText("\033[31m Result fetching error.");
        mysql_close(conn);
        return;
    }

    MYSQL_ROW Row;
    bool Check_menu_ID = false;
    while (Row = mysql_fetch_row(Result))
    {
        if (Row[0] != nullptr && Row[0] == Current_user_ID)
        {
            Check_menu_ID = true;
            std::string Delete_row_query =
                "DELETE FROM MENU WHERE ID = '" + Current_user_ID + "'";
            if (mysql_query(conn, Delete_row_query.c_str()))
            {
                system("clear");
                centeredText("\033[31m Query execution error: " + std::string(mysql_error(conn)));
                mysql_close(conn);
                return;
            }
        }
    }

    mysql_close(conn);

    if (!Check_menu_ID)
    {
        system("clear");
        centeredText("\033[31m Sorry, you didn't add a menu!");
        firstPage();
    }
    else
    {
        header();
        centeredText("Your menu has been deleted successfully!");
        firstPage();
    }
}

void Menu::firstPage()
{
    int answer;
    Menu obj;
    header() ; 
    centeredText(" * Press 1 to Add Menu");
    centeredText(" * Press 2 to Edit Menu");
    centeredText(" * Press 3 to Show Menu");
    centeredText(" * Press 4 to Delete Menu");
    centeredText(" * Press 5 to Take Order");
    centeredText(" * Press 6 to EXIT");

    std::cout << "\t Enter your choice: ";

    std::cin >> answer;
    switch (answer)
    {
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
            "Tashir_ID"); // TODO change this to the current user id
        break;
    case 6:
        centeredText("Thank you!");
        break;
    default:
        system("clear");
        centeredText("\033[33m Select from the options given above");
        firstPage();
    }
}
