#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <iomanip> 
#include <sys/ioctl.h>
#include <unistd.h>

#include "/usr/include/mysql/mysql.h"

class Menu {
   public:
    std::string Dish_name, Menu_ID;
    int Oil_count, Frying_level;

    Menu();
    Menu(const std::string Rest_name_menu_ID, const std::string Dish_name,
         const int Oil_count, const int Frying_level);

    void addMenu(std::string Current_user_ID);
    void editMenu(std::string Current_user_ID);
    void deleteMenu(std::string Current_user_ID);
    void showMenu(std::string Current_user_ID);
    void firstPage();
    void centeredText(const std::string &text ) ;
    int getTerminalWidth( ) ;
    void header() ; 
    std::pair<MYSQL_RES *, int> fetchUser(std::string Current_user_ID);
};