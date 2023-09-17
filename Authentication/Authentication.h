#include <iostream>
#include <string>

class Authentication {
   public:
    void login();
    void forgot();
    void authPage();
    void firstpage_two();
    void profile();
    void registration();
    bool lettersAndNumbers(std::string& password);
    std::string username, password;
    int maxOilCount;
};