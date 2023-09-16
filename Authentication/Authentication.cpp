#include "../Authentication/Authentication.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <random>

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

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    std::string *data = static_cast<std::string *>(stream);
    if (size * nmemb < 1)
        return 0;

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

void Authentication::profile() {
    std::cout << "\t\t\t\t --------- Hello!--------- \n\n";
    std::cout << "\t\t\t\t  Here is your Profile \n\n\n";
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
    char serverUsername[25] = "sql8646145";
    char serverPassword[25] = "z9nFFL1Han";
    char database[25] = "sql8646145";

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
    char serverUsername[25] = "sql8646145";
    char serverPassword[25] = "z9nFFL1Han";
    char database[25] = "sql8646145";

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
        while ((row = mysql_fetch_row(result))) {
            if (UserName == row[1] && Password == row[2]) {
                exist = true;
                std::cout << "\n\n\t\t\t\t Welcome to your profile! \n\n\n";
                profile();
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
    std::cout << "\n\n\n\t\t\t Forgot your password? We can help you!\n\n";
    std::cout << "\t\t * Press 1 to change your password by email" << std::endl;
    std::cout << "\t\t * Press 2 to  go to the main page" << std::endl;
    std::cout << "\t\t Enter your choice: \n";
    std::cout << "\t\t __ ";
    std::cin >> choice;

    switch (choice) {
        case 1: {
            char server[26] = "sql8.freesqldatabase.com";
            char username[15] = "sql8646145";
            char password[15] = "z9nFFL1Han";
            char database[15] = "sql8646145";
            std::string chooseUserName, recipient, choosePassword;
            bool foundUsername = false;
            std::cout << "\x1B[2J\x1B[H";

            MYSQL *conn = mysql_init(NULL);

            if (mysql_real_connect(conn, server, username, password, database, 0,
                                nullptr, 0) == nullptr) {
                std::cerr << "Unable to connect with MySQL server\n";
                mysql_close(conn);
                return;
            }

            std::string Select_column_query = "SELECT Username, Email, Password FROM RESTAURANTS";
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

            
            std::cout << "Now enter the username with which you registered.\nAnswer: ";
            std::cin >> chooseUserName;

            MYSQL_ROW firstRow;
            while ((firstRow = mysql_fetch_row(firstResult)))
            {
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
                std::string body = "<html><head><style>body {font-family: Arial, sans-serif;}.large-font {font-size: 18px;}.very-large-font {font-size: 20px;}.move-right {margin-left: 650px;}.for-number {font-size: 30px;color: green;}.for-logo-text {font-size: 35px;}</style></head><body><p class='large-font' style=text-align: center;'>Hi " + chooseUserName + ",</p><p class='very-large-font' style=text-align: center;'><b>We hope this message finds you well. It seems you've requested a password reset for your Oil Management account. Your account security is important to us, and we're here to assist you in regaining access to your account. To proceed with the password reset, please enter the following number in the terminal.</b></p><p class='for-number' style='text-align: center;'><b>" + std::to_string(randomNumber) + "</b></p><p class='for-logo-text' style='text-align: center;'>The Blago Team</p><img src='https://imgtr.ee/images/2023/09/15/8983974c04811e5f05bd79996105d13f.png' alt='Image' style='width: 200px; height: 125px;' class='move-right'><p class='large-font'>Best Regards.</p></body></html>";
                
                CURL *curl = curl_easy_init();

                if (!curl) {
                    std::cerr << "Error initializing libcurl." << std::endl;
                    return;
                }

                std::string emailData = "To: " + recipient + "\r\n"
                                    "From: " + sender + "\r\n"
                                    "Subject: " + subject + "\r\n"
                                    "MIME-Version: 1.0\r\n"
                                    "Content-Type: text/html; charset=utf-8\r\n\r\n" + body;

                struct curl_slist *recipients = NULL;
                recipients = curl_slist_append(recipients, recipient.c_str());

                curl_easy_setopt(curl, CURLOPT_USERNAME, "sargsyanvahag6@gmail.com");    
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

                // std::cout << "\033[2J\033[1;1H"; 

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

                std::cout << "We have sent you an email. Enter the code.\n";

                while (chance != 0)
                {
                    std::cout << "You have " << chance << " chances to enter!!!\nAnswer: ";
                    std::cin >> code;
                    if (code == randomNumber) {
                        std::cout << "\x1B[2J\x1B[H";
                        std::string newPassword; 
                        std::cout << "\t\t\t Now enter a new password: " << std::endl;
                        std::cout
                            << "\t\t\t Your  password must  have letters, numbers and minimum 6 "
                            "symbols "
                            << std::endl;
                        std::cout << "\t\t\t _ ";
                        std::cin >> newPassword;

                        while (true) {
                            if (lettersAndNumbers(newPassword)) {
                                std::cout << "\t\t\t Strong password! \n\n\n ";
                                break;
                            } 
                            else {
                                std::cout << "\t\t\t Password have to  contain both  letters and "
                                            "numbers. \n";
                                std::cout << "\t\t\t __ ";
                                std::cin >> newPassword;
                            }
                        }
                        
                        std::string updateRowQuery = "UPDATE RESTAURANTS SET Password = '" + newPassword + "' WHERE Username = '" + chooseUserName + "'";

                        if (mysql_query(conn, updateRowQuery.c_str())) {
                            std::cerr << "Query execution error." << std::endl;
                            mysql_close(conn);
                            return;
                        }

                        std::cout << "Your password has been successfully changed!!!\n";
                        authPage();
                        break;
                    }
                    else {
                        --chance;
                    }
                }

                if (chance == 0) {
                    std::cout << "Sorry, you are no longer able to enter. In case of questions, you can contact the Blago Team company.\n";
                    authPage();
                }
            }
            else {
                std::cout << "\x1B[2J\x1B[H";
                std::cout << "There is no such user in the system, you can try again!!!\n";
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
