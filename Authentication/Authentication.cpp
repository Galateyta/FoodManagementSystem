#include "../Authentication/Authentication.h"

#include <fstream>
#include <iostream>
#include <string>

#include "../Profile/Profile.h"
#include "../utils/Utils.h"
#include "/usr/include/mysql/mysql.h"

void Authentication::authPage() {
    std::cout << "\t\t\t ___________________________________ \n";
    std::cout << "\t\t\t ***             WELCOME!        *** \n";
    std::cout << "\t\t\t    ______________________________   \n";
    std::cout << "\t\t\t     Used Oil Management Programm    \n\n";

    int choice;
    std::cout << "\t * Press 1 to Login    " << std::endl;
    std::cout << "\t * Press 2 to Register " << std::endl;
    std::cout << "\t * Press 3 if you forgot your password " << std::endl;
    std::cout << "\t * Press 4 to EXIT \n\n";
    std::cout << "\t Enter your  choice: ";
    std::cin >> choice;
    std::cout << std::endl;

    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            registration();
            break;
        case 3:
            forgot();
            break;
        case 4:
            std::cout << "\t\t\t Thank you! \n\n";
            break;
        default:
            std::cout << "\x1B[2J\x1B[H";
            std::cout << "\t\t\t Select from the options given above \n";
            authPage();
    }
}

// ___________________________________________________________ //

bool Authentication::lettersAndNumbers(std::string& password) {
    bool hasLetters = false;
    bool hasNumbers = false;
    for (char c : password) {
        if (isalpha(c)) {
            hasLetters = true;
        } else if (isdigit(c)) {
            hasNumbers = true;
        }
        if (hasLetters && hasNumbers) {
            break;
        }
    }
    return hasLetters && hasNumbers;
}
// _________________________________________________________________ //

void Authentication::registration() {
    std::string RestName, Address, Email;
    int a;
    char userrole[20] = "User";
    std::cout << "\x1B[2J\x1B[H";

    std::cout << "\t\t\t Create a USERNAME: " << std::endl;
    std::cout << "\t\t\t _ ";
    std::cin >> username;
    // ************************ //

    char server[26] = "sql8.freesqldatabase.com";
    char serverUsername[15] = "sql8646145";
    char serverPassword[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL* conn = mysql_init(NULL);

    if (conn == NULL) {
        printf("MySQL initialization failed");
        return;
    }

    if (mysql_real_connect(conn, server, serverUsername, serverPassword,
                           database, 0, NULL, 0) == NULL) {
        printf("Unable to connect with MySQL server\n");
        mysql_close(conn);
        return;
    }

    if (mysql_query(conn, "SELECT * FROM RESTAURANTS")) {
        std::cout << "Unable to  connect with MySQL server\n";
        mysql_close(conn);
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        std::cout << "Result fetching error." << std::endl;
        mysql_close(conn);
        return;
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; ++i) {
            if (username == row[i]) {
                std::cout << "\t\t\t This username already exist! Try again! "
                          << std::endl;
                std::cout << "\t\t\t __ ";
                std::cin >> username;
            }
        }
    }

    mysql_free_result(result);
    mysql_close(conn);

    // ************************* //
    std::cout << "\n\n\t\t\t Enter your public food place name: " << std::endl;
    std::cout << "\t\t\t _ ";
    std::cin.ignore();
    std::getline(std::cin, RestName);

    std::cout << "\n\n\t\t\t Create a PASSWORD: " << std::endl;
    std::cout
        << "\t\t\t Your  password must  have letters, numbers and minimum 6 "
           "symbols "
        << std::endl;
    std::cout << "\t\t\t _ ";
    std::cin >> password;

    while (password.length() < 6) {
        std::cout
            << "\t\t\t Your password must have minimum 6 symbols . Try again "
            << std::endl;
        std::cout << "\t\t\t Create a PASSWORD: ";
        std::cout << "\t\t\t _ ";
        std::cin >> password;
    }
    while (true) {
        if (lettersAndNumbers(password)) {
            std::cout << "\t\t\t Strong password! \n\n\n ";
            break;
        } else {
            std::cout << "\t\t\t Password have to  contain both  letters and "
                         "numbers. \n";
            std::cout << "\t\t\t __ ";
            std::cin >> password;
        }
    }

    std::cout << "\n\n\t\t\t Enter your address: " << std::endl;
    std::cout << "\t\t\t _ ";
    std::cin.ignore();
    std::getline(std::cin, Address);

    std::cout << "\n\n\t\t\t Enter your email: " << std::endl;
    std::cout << "\t\t\t _ ";
    std::cin >> Email;

    std::cout << "\n\n\t\t\t Enter your used oil storage limit: " << std::endl;
    std::cout << "\t\t\t _ ";
    std::cin >> maxOilCount;
    // _________________________________________ //

    std::cout << "\x1B[2J\x1B[H";
    std::cout << "\n\t\t\t Your registration is done! \n";

    MYSQL* connn = mysql_init(NULL);

    if (connn == NULL) {
        printf("MySQL initialization failed");
        return;
    }

    if (mysql_real_connect(connn, server, serverUsername, serverPassword,
                           database, 0, NULL, 0) == NULL) {
        printf("Unable to connect with MySQL server\n");
        mysql_close(connn);
        return;
    }

    std::string userID = generateUniqueID();

    std::string queryString =
        "INSERT INTO RESTAURANTS (ID, Username, Password, RestaurantName, "
        "Address, "
        "Email, Role, MaxOilCount, CurrentOilCount, Coefficient, Frequency) "
        "VALUES ('" +
        userID + "','" + username + "','" + password + "','" + RestName +
        "','" + Address + "','" + Email + "','" + userrole + "','" +
        std::to_string(maxOilCount) + "','" + "0" + "','" + "0" + "','" +
        "0"
        "')";
    std::string insertQuery = queryString;

    if (mysql_query(connn, insertQuery.c_str())) {
        mysql_close(connn);
        return;
    }

    std::cout << "\t\t\t Now you can login to your profile. \n\n\n";
    authPage();
}

// ________________________________________________________ //

void Authentication::login() {
    int count;
    std::string UserName, Password;

    std::cout << "\x1B[2J\x1B[H";

    // *************************************** //
    char server[26] = "sql8.freesqldatabase.com";
    char serverUsername[15] = "sql8646145";
    char serverPassword[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL* conn = mysql_init(NULL);

    if (conn == NULL) {
        printf("MySQL initialization failed");
        return;
    }

    if (mysql_real_connect(conn, server, serverUsername, serverPassword,
                           database, 0, NULL, 0) == NULL) {
        printf("Unable to connect with MySQL server\n");
        mysql_close(conn);
        return;
    }

    bool exist = false;
    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;
    while (!exist) {
        std::cout << "\t\t\t Enter username: \n";
        std::cout << "\t\t\t __ ";
        std::cin >> UserName;
        std::cout << "\t\t\t Enter password: \n";
        std::cout << "\t\t\t __ ";
        std::cin >> Password;
        std::string selectAllQuery = "SELECT * FROM RESTAURANTS";

        if (mysql_query(conn, "SELECT * FROM RESTAURANTS")) {
            std::cout << "Unable to  connect with MySQL server\n";
            mysql_close(conn);
            return;
        }

        MYSQL_RES* result = mysql_store_result(conn);
        if (result == NULL) {
            std::cout << "Result fetching error." << std::endl;
            mysql_close(conn);
            return;
        }
        Profile profile;

        while ((row = mysql_fetch_row(result))) {
            if (UserName == row[1] && Password == row[2]) {
                exist = true;
                std::cout << "\n\n\t\t\t\t Welcome to your profile! \n\n\n";
                profile.showProfilePage();
                break;
            } else {
                exist = false;
            }
        }
        if (exist) {
            mysql_free_result(result);
            mysql_close(conn);
            break;
        } else {
            std::cout << "Username or password is incorrect!. Try again! \n\n";
            continue;
        }
    }
}
// ******************************************* //

// _______________________________________________ //

void Authentication::forgot() {
    int choice;
    std::cout << "\x1B[2J\x1B[H";
    std::cout << "\n\n\n\t\t\t Forgot your password? We can help you!\n\n";
    std::cout << "\t\t * Press 1 to change your password by email" << std::endl;
    std::cout << "\t\t * Press 2 to  go to the main page" << std::endl;
    std::cout << "\t\t Enter your choice: \n";
    std::cout << "\t\t __ ";
    std::cin >> choice;

    switch (choice) {
        case 1: {
            int count = 0;
            std::string Fuserid, Fid, Fpass;
            std::cout << "\t\t\t Enter the USERNAME: ";
            std::cin >> Fuserid;
            std::ifstream f3("data.xls");
            if (f3.is_open()) {
                while (f3 >> Fid >> Fpass) {
                    if (Fid == Fuserid) {
                        count = 1;
                    }
                }
                f3.close();
            }
            if (count == 1) {
                std::cout << "\t\t Your account is found\n";
                std::cout << "\t\t Your  password is: " << Fpass;  // TODO
                std::cout << "\n\t\t Your  account is not  found!\n";
                authPage();
            }
            break;
        }
        case 2: {
            authPage();
        }
        default:
            std::cout << "\t\t Wrong choice! Try  again!";
    }
}
