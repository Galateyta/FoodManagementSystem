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
    //size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);
    std::string generateID();
    std::string username, password;
    int maxOilCount;
};