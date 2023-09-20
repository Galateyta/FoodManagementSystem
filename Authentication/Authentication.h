#include <iostream>
#include <string>
#include <tabulate/table.hpp>

using namespace tabulate;
class Authentication {
   public:
    void login();
    void forgot();
    void authPage();
    void profile();
    void registration();
    bool lettersAndNumbers(std::string& password);
    std::string username, password;
    int maxOilCount;
    void loadingAnimation();
    void header();
    void loading();
    void centeredText(const std::string& text);
    int getTerminalWidth();
    void adminPage();
    void centeredTextNoEndl(const std::string& text);
};