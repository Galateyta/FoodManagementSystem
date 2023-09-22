#include "../Authentication/Authentication.h"

#include <curl/curl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
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
    // cout << "\033[1;31mbold red text\033[0m\n";
    std::cout << std::setw(padding + textWidth)
              << "\033[1;40m" + text + "\033[0m\n"
              << std::endl;
}
void Authentication::centeredTextNoEndl(const std::string &text) {
    int terminalWidth = getTerminalWidth();

    int textWidth = text.length();
    int padding = (terminalWidth - textWidth) / 2;
    std::cout << std::setw(padding + textWidth)
              << "\033[1;40m" + text + "\033[0m";
}
void Authentication::header() {
    centeredText("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    centeredText("FOOD MANAGEMENT");
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
    Table authPageTable;
    Table choiceTable;
    header();
    int choice;
    authPageTable.add_row(
        {"1. Login", "2. Register", "3. Forgot Password", "4. Exit"});
    authPageTable[0]
        .format()
        .font_color(Color::yellow)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .width(40);
    std::cout << authPageTable << std::endl;
    choiceTable.add_row({"Enter Your Choice: "});
    choiceTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << choiceTable;
    std::cout << "\t";
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
            centeredText("Thank You!");
            break;
        default:
            centeredText("\033[31m Select from the options given above");
            sleep(2);
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
    Table usernameTable;
    usernameTable.add_row({"Enter Username: "});
    Table foodServiceTable;
    foodServiceTable.add_row({"Enter Food Service Name: "});
    Table passwordTable;
    passwordTable.add_row({"Enter Password: "});
    Table addressTable;
    addressTable.add_row({"Enter Address: "});
    Table emailTable;
    emailTable.add_row({"Enter Email: "});
    Table storageTable;
    storageTable.add_row({"Enter Your Oil Storage Limit: "});
    usernameTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << usernameTable;
    std::cout << "\t";
    std::cin >> username;
    std::cout << "\n" << std::endl;

    char server[26] = "sql11.freesqldatabase.com";
    char serverUsername[25] = "sql11647725";
    char serverPassword[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

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

    foodServiceTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << foodServiceTable;
    std::cout << "\t";
    std::cin.ignore();
    std::getline(std::cin, RestName);
    std::cout << "\n" << std::endl;

    passwordTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << passwordTable;
    std::cout << "\t";
    std::cin >> password;
    std::cout << "\n" << std::endl;

    // TODO hint - Your password must have letters, numbers and minimum 6
    // symbols

    while (password.length() < 6) {
        centeredText(
            "\033[31m Your password must have minimum 6 symbols. Try again");
        centeredText("\033[32m Create a PASSWORD:");
        std::cout << "\t\t\t _ ";
        std::cin >> password;
    }
    while (true) {
        if (lettersAndNumbers(password)) {
            break;
        } else {
            centeredText(
                "\033[31m Password have to contain both letters and "
                "numbers.");
            std::cout << "\t\t\t __ ";
            std::cin >> password;
        }
    }

    addressTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << addressTable;
    std::cout << "\t";
    std::cin.ignore();
    std::getline(std::cin, Address);
    std::cout << "\n" << std::endl;

    emailTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << emailTable;
    std::cout << "\t";
    std::cin >> Email;
    std::cout << "\n" << std::endl;

    storageTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << storageTable;
    std::cout << "\t";
    std::cin >> maxOilCount;
    std::cout << "\n" << std::endl;
    // _________________________________________ //

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
    system("clear");
    sleep(3);
    authPage();
}

// ________________________________________________________ //

void Authentication::login() {
    int count;
    std::string UserName, Password;

    char server[26] = "sql11.freesqldatabase.com";
    char serverUsername[25] = "sql11647725";
    char serverPassword[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";

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
    Table usernameTable;
    usernameTable.add_row({"Enter Username: "});
    Table passwordTable;
    passwordTable.add_row({"Enter Password: "});

    while (!exist) {
        usernameTable[0]
            .format()
            .background_color(Color::green)
            .font_align(FontAlign::center);
        std::cout << usernameTable;
        std::cout << "\t";
        std::cin >> UserName;

        std::cout << "\n" << std::endl;
        passwordTable[0]
            .format()
            .background_color(Color::red)
            .font_align(FontAlign::center);
        std::cout << passwordTable;
        std::cout << "\t";
        std::cin >> Password;

        std::string selectAllQuery = "SELECT * FROM RESTAURANTS";
        if (UserName == "admin" && Password == "admin") {
            adminPage();
            return;
        }
        if (mysql_query(conn, selectAllQuery.c_str())) {
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
            continue;
        }
    }
}

// _______________________________________________ //

static size_t read_callback(void *ptr, size_t size, size_t nmemb,
                            void *stream) {
    std::string *data = static_cast<std::string *>(stream);
    if (size * nmemb < 1) return 0;

    size_t curl_size = size * nmemb;
    size_t data_size = data->size();

    if (data_size > 0) {
        const char *read_data = data->c_str();
        size_t to_copy = std::min(curl_size, data_size);
        memcpy(ptr, read_data, to_copy);
        data->erase(0, to_copy);
        return to_copy;
    }

    return 0;
}

void Authentication::forgot() {
    Table forgotTable, choiceTable;
    int choice;
    centeredText("Forgot your password? We can help you!");
    forgotTable.add_row({"1. Change password by Email", "2. Back"});
    forgotTable[0]
        .format()
        .font_color(Color::yellow)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .width(40);

    std::cout << forgotTable << std::endl;

    choiceTable.add_row({"Enter your choice:"});
    choiceTable[0]
        .format()
        .background_color(Color::green)
        .font_align(FontAlign::center);
    std::cout << choiceTable;
    std::cout << "\t";
    std::cin >> choice;

    switch (choice) {
        case 1: {
            char server[26] = "sql11.freesqldatabase.com";
            char username[25] = "sql11647725";
            char password[25] = "zvyL1P8QXx";
            char database[25] = "sql11647725";
            std::string chooseUserName, recipient, choosePassword;
            bool foundUsername = false;
            std::cout << "\x1B[2J\x1B[H";

            MYSQL *conn = mysql_init(NULL);

            if (mysql_real_connect(conn, server, username, password, database,
                                   0, nullptr, 0) == nullptr) {
                std::cerr << "Unable to connect with MySQL server\n";
                mysql_close(conn);
                return;
            }

            std::string Select_column_query =
                "SELECT Username, Email, Password FROM RESTAURANTS";
            if (mysql_query(conn, Select_column_query.c_str())) {
                std::cerr << "Query execution error." << std::endl;
                mysql_close(conn);
                return;
            }

            MYSQL_RES *firstResult = mysql_store_result(conn);

            if (firstResult == nullptr) {
                std::cerr << "Result fetching error." << std::endl;
                mysql_close(conn);
                return;
            }
            Table usernameTable;
            usernameTable.add_row({"Enter your username:"});
            usernameTable[0]
                .format()
                .background_color(Color::green)
                .font_align(FontAlign::center);
            std::cout << usernameTable;
            std::cout << "\t";
            std::cin >> chooseUserName;

            MYSQL_ROW firstRow;
            while ((firstRow = mysql_fetch_row(firstResult))) {
                if (firstRow[0] != nullptr && firstRow[0] == chooseUserName) {
                    foundUsername = true;
                    recipient = firstRow[1];
                    choosePassword = firstRow[2];
                    break;
                }
            }

            mysql_free_result(firstResult);

            if (foundUsername) {
                std::random_device rd;
                std::mt19937 generator(rd());
                std::uniform_int_distribution<int> distribution(100000, 999999);

                int randomNumber = distribution(generator), chance = 3, code;

                if (mysql_query(conn, Select_column_query.c_str())) {
                    std::cerr << "Query execution error." << std::endl;
                    mysql_close(conn);
                    return;
                }

                MYSQL_RES *secondResult = mysql_store_result(conn);

                if (secondResult == nullptr) {
                    std::cerr << "Result fetching error." << std::endl;
                    mysql_close(conn);
                    return;
                }

                MYSQL_ROW secondRow;

                std::string sender = "sargsyanvahag6@gmail.com";
                std::string subject = "Blago Team";
                std::string body =
                    "<html><head><style>body {font-family: Arial, "
                    "sans-serif;}.large-font {font-size: "
                    "18px;}.very-large-font {font-size: 20px;}.move-right "
                    "{margin-left: 650px;}.for-number {font-size: 30px;color: "
                    "green;}.for-logo-text {font-size: "
                    "35px;}</style></head><body><p class='large-font' "
                    "style=text-align: center;'>Hi " +
                    chooseUserName +
                    ",</p><p class='very-large-font' style=text-align: "
                    "center;'><b>We hope this message finds you well. It seems "
                    "you've requested a password reset for your Oil Management "
                    "account. Your account security is important to us, and "
                    "we're here to assist you in regaining access to your "
                    "account. To proceed with the password reset, please enter "
                    "the following number in the terminal.</b></p><p "
                    "class='for-number' style='text-align: center;'><b>" +
                    std::to_string(randomNumber) +
                    "</b></p><p class='for-logo-text' style='text-align: "
                    "center;'>The Blago Team</p><img "
                    "src='https://imgtr.ee/images/2023/09/15/"
                    "8983974c04811e5f05bd79996105d13f.png' alt='Image' "
                    "style='width: 200px; height: 125px;' "
                    "class='move-right'><p class='large-font'>Best "
                    "Regards.</p></body></html>";

                CURL *curl = curl_easy_init();

                if (!curl) {
                    std::cerr << "Error initializing libcurl." << std::endl;
                    return;
                }

                std::string emailData =
                    "To: " + recipient +
                    "\r\n"
                    "From: " +
                    sender +
                    "\r\n"
                    "Subject: " +
                    subject +
                    "\r\n"
                    "MIME-Version: 1.0\r\n"
                    "Content-Type: text/html; charset=utf-8\r\n\r\n" +
                    body;

                struct curl_slist *recipients = NULL;
                recipients = curl_slist_append(recipients, recipient.c_str());

                curl_easy_setopt(curl, CURLOPT_USERNAME,
                                 "sargsyanvahag6@gmail.com");
                curl_easy_setopt(curl, CURLOPT_PASSWORD, "ijtmcqbdakiegezn");
                curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
                std::cout << "\033[2J\033[1;1H";
                curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com");
                std::cout << "\033[2J\033[1;1H";
                curl_easy_setopt(curl, CURLOPT_PORT, 465);
                curl_easy_setopt(curl, CURLOPT_MAIL_FROM, sender.c_str());
                curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
                curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
                curl_easy_setopt(curl, CURLOPT_READDATA, &emailData);
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                CURLcode res = curl_easy_perform(curl);
                if (res != CURLE_OK) {
                    std::cout << "The email you entered was incorrect.\n";
                    curl_slist_free_all(recipients);
                    curl_easy_cleanup(curl);
                    authPage();
                    return;
                }
                curl_slist_free_all(recipients);
                curl_easy_cleanup(curl);

                Table codeTable;
                codeTable.add_row(
                    {"We have sent you an email. Enter the code."});
                codeTable[0]
                    .format()
                    .background_color(Color::green)
                    .font_align(FontAlign::center);
                std::cout << codeTable;
                std::cout << "\t" << std::endl;

                while (chance != 0) {
                    centeredText("You have " + std::to_string(chance) +
                                 " more chances.");
                    std::cin >> code;
                    if (code == randomNumber) {
                        std::cout << "\x1B[2J\x1B[H";
                        std::string newPassword;
                        Table newPassTable;
                        newPassTable.add_row({"Now enter a new password"});
                        newPassTable[0]
                            .format()
                            .background_color(Color::green)
                            .font_align(FontAlign::center);
                        std::cout << newPassTable << std::endl;
                        std::cout << "\t";
                        centeredText(
                            "Your password must have letters, numbers and "
                            "minimum 6 symbols");
                        std::cin >> newPassword;

                        while (true) {
                            if (lettersAndNumbers(newPassword)) {
                                break;
                            } else {
                                centeredText(
                                    "Password have to  contain both letters "
                                    "and numbers.");
                                std::cout << "\t __ ";
                                std::cin >> newPassword;
                            }
                        }

                        std::string updateRowQuery =
                            "UPDATE RESTAURANTS SET Password = '" +
                            newPassword + "' WHERE Username = '" +
                            chooseUserName + "'";

                        if (mysql_query(conn, updateRowQuery.c_str())) {
                            std::cerr << "Query execution error." << std::endl;
                            mysql_close(conn);
                            return;
                        }

                        centeredText(
                            "Your password has been successfully changed!!!");
                        sleep(4);
                        system("clear");
                        authPage();
                        break;
                    } else {
                        --chance;
                    }
                }

                if (chance == 0) {
                    centeredText(
                        "\033[31m Sorry, you are no longer able to enter. In "
                        "case of questions, you can contact the Blago "
                        "Team company.");
                    sleep(4);
                    system("clear");
                    authPage();
                }
            } else {
                std::cout << "\x1B[2J\x1B[H";
                std::cout << "There is no such user in the system, you can try "
                             "again!!!\n";
                mysql_close(conn);
                forgot();
            }
            break;
        }
        case 2: {
            authPage();
            break;
        }
        default:
            std::cout << "\t\t Wrong choice! Try  again!";
            forgot();
            break;
    }
}

void space(int num) {
    for (int i = 0; i <= num; i++) {
        std::cout << " ";
    }
}
void Authentication::adminPage() {
    int choise = 0;
    char server[26] = "sql11.freesqldatabase.com";
    char serverUsername[25] = "sql11647725";
    char serverPassword[25] = "zvyL1P8QXx";
    char database[25] = "sql11647725";
    std::string selectColumnsQuery;

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
    selectColumnsQuery =
        "SELECT RestaurantName, MaxOilCount, "
        "CurrentOilCount, Coefficient, Frequency FROM RESTAURANTS";
    if (mysql_query(conn, selectColumnsQuery.c_str())) {
        std::cerr << "Query execution error: " << mysql_error(conn)
                  << std::endl;
        mysql_close(conn);
        return;
    }
    MYSQL_RES *result = mysql_store_result(conn);

    Table adminTable;
    centeredText("Hello ADMIN\n");
    adminTable.add_row({"1. Food Services Analytics", "2. Exit"});
    adminTable[0]
        .format()
        .font_color(Color::yellow)
        .font_align(FontAlign::center)
        .font_style({FontStyle::bold})
        .width(40);
    std::cout << adminTable << std::endl;
    std::cin >> choise;
    system("clear");
    Table analytics;
    switch (choise) {
        case 1:
            if (result == nullptr) {
                std::cerr << "Result fetching error." << std::endl;
                mysql_close(conn);
                return;
            }
            MYSQL_ROW row;
            std::cout << "\t\t\t\t\t+---------------+-------------+------------"
                         "-----+-------------+-----------+"
                      << std::endl;
            std::cout << "\t\t\t\t\t\x1B[33m|RestaurantName | MaxOilCount | "
                         "CurrentOilCount | Coefficient | Frequency |"
                      << std::endl;
            std::cout << "\t\t\t\t\t+---------------+-------------+------------"
                         "-----+-------------+-----------+"
                      << std::endl;
            while ((row = mysql_fetch_row(result))) {
                for (int i = 0; i < mysql_num_fields(result); ++i) {
                    if (row[i] != nullptr) {
                        if (i == 0) {
                            std::cout << "\t\t\t\t\t|";
                            std::size_t length = std::strlen(row[i]);
                            std::cout << row[i];
                            space(14 - length);
                            std::cout << "|";
                        } else if (i % 4 == 1) {
                            std::size_t length = std::strlen(row[i]);
                            std::cout << row[i];
                            space(12 - length);
                            std::cout << "|";
                        } else if (i % 4 == 2) {
                            std::size_t length = std::strlen(row[i]);
                            std::cout << row[i];
                            space(16 - length);
                            std::cout << "|";
                        } else if (i % 4 == 3) {
                            std::size_t length = std::strlen(row[i]);
                            std::cout << row[i];
                            space(12 - length);
                            std::cout << "|";
                        } else if (i % 4 == 0) {
                            std::size_t length = std::strlen(row[i]);
                            std::cout << row[i];
                            space(10 - length);
                            std::cout << "|";
                            std::cout << std::endl;
                            std::cout
                                << "\t\t\t\t\t+---------------+-------------+--"
                                   "---------------+-------------+-----------+"
                                << std::endl;
                        }
                    }
                }
            }
            break;
        case 2:
            return;
            break;
        default:
            mysql_free_result(result);
            mysql_close(conn);
            system("clear");
            centeredText("\033[31m Select from the options given above");
            sleep(2);
            adminPage();
            break;
    }
    adminPage();

    mysql_free_result(result);
    mysql_close(conn);
}
