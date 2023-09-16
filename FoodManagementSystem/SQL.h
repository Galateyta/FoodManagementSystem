#ifndef SQL_H
#define SQL_H

#include "/usr/include/mysql/mysql.h"
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
class SQL
{
private:
    string  password ; 
    string  adminname ; 
    char server[30] = "sql8.freesqldatabase.com";
    char username[25] = "sql8646145";
    char SQLpassword[25] = "z9nFFL1Han";
    char database[25] = "sql8646145";
    MYSQL *conn = mysql_init(NULL);
 

public:
    
    SQL();
    bool SQLconnection();
    void SQLgetAdminPassLogin() ; 
    void SQLgetOrder() ; 
    void alignment (string & passWord , string & adminName ) ;
};


#endif // SQL_H