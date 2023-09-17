#include "../Authentication/Authentication.h"

#include <sys/ioctl.h>
#include <unistd.h>

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include "../Profile/Profile.h"
#include "../globalStates/GlobalStates.h"
#include "../utils/Utils.h"
#include "/usr/include/mysql/mysql.h"

int Authentication::getTerminalWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

void Authentication::centeredText(const std::string &text) {
    int terminalWidth = getTerminalWidth();

    int textWidth = text.length();
    int padding = (terminalWidth - textWidth) / 2;

    std::cout << std::setw(padding + textWidth) << text << std::endl;
}
void Authentication::header() {
    system("clear");
    centeredText("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    centeredText("\033[32m FOOD MANAGEMENT ");
};

void Authentication::loadingAnimation() {
    int totalIterations = 100;

    for (int i = 0; i <= totalIterations; ++i) {
        float progress = static_cast<float>(i) / totalIterations;

        header();

        std::cout << "Loading:[ ";

        int barWidth = getTerminalWidth() - 20;
        int progressWidth = static_cast<int>(barWidth * progress);

        for (int j = 0; j < barWidth; ++j) {
            if (j <= progressWidth) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }

        std::cout << " ] " << int(progress * 100.0) << "%" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    system("clear");
    header();
    centeredText("Loading complete!");
    sleep(1);
}
void Authentication::loading() {
    loadingAnimation();
    header();
    authPage();
}
void Authentication::authPage() {
    header();
    int choice;
    std::cout << "\t\033[32m * Press 1 to Login    " << std::endl;
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
            system("clear");
            centeredText("Thank You!");
            break;
        default:
            system("clear");
            centeredText("\033[31m Select from the options given above");
            sleep(2);
            system("clear");
            authPage();
    }
}

void Authentication::profile() { centeredText(" Here is your Profile"); }
// ___________________________________________________________ //

bool Authentication::lettersAndNumbers(std::string &password) {
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
    system("clear");

    centeredText(" Create a USERNAME:");
    std::cout << "\t\t\t _ ";
    std::cin >> username;
    // ************************ //

    char server[26] = "sql8.freesqldatabase.com";
    char serverUsername[25] = "sql8644761";
    char serverPassword[25] = "M8c6DWvEMr";
    char database[25] = "sql8644761";

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        printf("\033[31m MySQL initialization failed");
        return;
    }

    if (mysql_real_connect(conn, server, serverUsername, serverPassword,
                           database, 0, NULL, 0) == NULL) {
        printf("\033[31m Unable to connect with MySQL server\n");
        mysql_close(conn);
        return;
    }

    if (mysql_query(conn, "SELECT * FROM RESTAURANTS")) {
        std::cout << "\033[31m Unable to  connect with MySQL server\n";
        mysql_close(conn);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        std::cout << "\033[31m Result fetching error." << std::endl;
        mysql_close(conn);
        return;
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; ++i) {
            if (username == row[i]) {
                centeredText(
                    "\033[31m This username already exist! Try again! ");
                std::cout << "\t\t\t __ ";
                std::cin >> username;
            }
        }
    }

    mysql_free_result(result);
    mysql_close(conn);

    // ************************* //
    centeredText("\033[32m Enter your public food place name: ");
    std::cout << "\t\t\t _ ";
    std::cin.ignore();
    std::getline(std::cin, RestName);

    centeredText("Create a PASSWORD: ");
    centeredText(
        "Your  password must  have letters, numbers and minimum 6 symbols");
    std::cout << "\t\t\t _ ";
    std::cin >> password;

    while (password.length() < 6) {
        centeredText(
            "\033[31m Your password must have minimum 6 symbols . Try again ");
        centeredText("\033[32m Create a PASSWORD:");
        std::cout << "\t\t\t _ ";
        std::cin >> password;
    }
    while (true) {
        if (lettersAndNumbers(password)) {
            centeredText("Strong password! ");
            break;
        } else {
            centeredText(
                "\033[31m Password have to  contain both  letters and "
                "numbers.");
            std::cout << "\t\t\t __ ";
            std::cin >> password;
        }
    }

    centeredText("\033[32m Enter your address: ");
    std::cout << "\t\t\t _ ";
    std::cin.ignore();
    std::getline(std::cin, Address);

    centeredText("Enter your email: ");
    std::cout << "\t\t\t _ ";
    std::cin >> Email;

    centeredText("Enter your used oil storage limit: ");
    std::cout << "\t\t\t _ ";
    std::cin >> maxOilCount;
    // _________________________________________ //

    system("clear");
    centeredText("Your registration is done!");

    MYSQL *connn = mysql_init(NULL);

    if (connn == NULL) {
        printf("\033[31m MySQL initialization failed");
        return;
    }

    if (mysql_real_connect(connn, server, serverUsername, serverPassword,
                           database, 0, NULL, 0) == NULL) {
        printf("\033[31m Unable to connect with MySQL server\n");
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

    centeredText(" Now you can login to your profile. ");
    sleep(2);
    system("clear");
    authPage();
}

// ________________________________________________________ //

void Authentication::login() {
    int count;
    std::string UserName, Password;

    system("clear");

    // *************************************** //
    char server[26] = "sql8.freesqldatabase.com";
    char serverUsername[15] = "sql8646145";
    char serverPassword[15] = "z9nFFL1Han";
    char database[15] = "sql8646145";

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        centeredText("\033[31m MySQL initialization failed");
        return;
    }

    if (mysql_real_connect(conn, server, serverUsername, serverPassword,
                           database, 0, NULL, 0) == NULL) {
        centeredText("\033[31m Unable to connect with MySQL server\n");
        mysql_close(conn);
        return;
    }

    bool exist = false;
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row;
    while (!exist) {
        std::cout << "\t\t\t Enter username:  ";
        std::cin >> UserName;
        std::cout << "\t\t\t Enter password:  ";
        std::cin >> Password;
        std::string selectAllQuery = "SELECT * FROM RESTAURANTS";

        if (mysql_query(conn, "SELECT * FROM RESTAURANTS")) {
            centeredText("\033[31m Unable to  connect with MySQL server\n");
            mysql_close(conn);
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            centeredText("\033[31m Result fetching error.");
            mysql_close(conn);
            return;
        }
        Profile profile;

        while ((row = mysql_fetch_row(result))) {
            if (UserName == row[1] && Password == row[2]) {
                GlobalStates globalStates(row[0]);
                exist = true;
                centeredText("\033[32m Welcome to your profile!");
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
            centeredText(
                "\033[31m Username or password is incorrect!. Try again! \n\n");
            system("clear");
            continue;
        }
    }
}

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
