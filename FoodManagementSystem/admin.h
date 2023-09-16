#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include "/usr/include/mysql/mysql.h"
#include <chrono>
#include <thread>
#include <cstdlib>
#include <unistd.h>
#include <iomanip> 
#include <sys/ioctl.h>
#include <unistd.h>
using namespace std;

class Admin {
private:
    const string userName;
    const string passWord;

public:
    Admin ( )  ; 
    Admin(const string& username, const string& password)
        : userName(username), passWord(password) {}

    bool login(const string& enteredPassword ,const string & enteredAdminname) {
        if( passWord == enteredPassword && userName == enteredAdminname){
            return true; 
        } else return false ;
    }
    
    void adminMainPage() ; 
    void loadingAnimation() ;
    void enterPassLog(string & enteredPass, string &enteredName ) ; 
    void checkingConnSQL() ;
    void adminLogin() ;
    
    

};

#endif // ADMIN_H