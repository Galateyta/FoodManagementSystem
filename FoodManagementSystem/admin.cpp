#include "admin.h"
#include "SQL.h"

#include <cstring>
using namespace std;

int getTerminalWidth()
{
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  return size.ws_col;
}
void centeredText(const string &text)
{

  int terminalWidth = getTerminalWidth();

  int textWidth = text.length();
  int padding = (terminalWidth - textWidth) / 2;

  cout << setw(padding + textWidth) << text<< endl ; ;
}

void header()
{
  system("clear");
  centeredText("\033[32m ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  centeredText("FOOD MANAGEMENT ");
}
void Admin::checkingConnSQL()
{
  Admin obj1;
  SQL obj2;
  header();
  centeredText("Checking connection whit SQL databasa , please white ...");
  sleep(2);

  if (obj2.SQLconnection() == true)
  {
    obj1.loadingAnimation();
    obj1.adminLogin();
  }
}

SQL::SQL() {}
bool SQL::SQLconnection()
{
  if (mysql_real_connect(conn, server, username, SQLpassword, database, 0, nullptr, 0) == nullptr)
  {
    centeredText("\033[31m Unable to connect with MySQL server");
    mysql_close(conn);
    return false;
  }

  if (mysql_query(conn, "SELECT * FROM ADMIN"))
  {
    centeredText("\033[31m Query execution error.");
    return false;
  }
  MYSQL_RES *result = mysql_store_result(conn);
  if (result == nullptr)
  {
    centeredText("\033[31m Result fetching error.");
    mysql_close(conn);
    return false;
  }
  else
  {
    centeredText("Your connection is successful!!!");
    return true;
  }
};
void SQL::SQLgetAdminPassLogin()

{
  if (mysql_real_connect(conn, server, username, SQLpassword, database, 0, nullptr, 0) == nullptr)
  {
    centeredText("\033[31m Unable to connect with MySQL server");
    return;
  }

  if (mysql_query(conn, "SELECT * FROM ADMIN"))
  {
    centeredText("\033[31m Query execution error.");
    mysql_close(conn);
    return;
  }

  MYSQL_RES *result = mysql_store_result(conn);
  if (result == nullptr)
  {
    centeredText("\033[31m Result fetching error.");
    mysql_close(conn);
    return;
  }

  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result)))
  {
    for (int i = 0; i < num_fields; ++i)
    {
      if (row[i] != nullptr && i == 0)
      {
        adminname = row[i];
      }
      else if (row[i] != nullptr && i == 1)
      {
        password = row[i];
      }
    }
    std::cout << std::endl;
  }

  mysql_free_result(result);
  mysql_close(conn);
}
void space(int num)
{
  for (int i = 0; i <= num; i++)
  {
    std::cout << " ";
  }
}
void SQL::SQLgetOrder()

{

  if (mysql_real_connect(conn, server, username, SQLpassword, database, 0, nullptr, 0) == nullptr)
  {
    centeredText("\033[31m Unable to connect with MySQL server");
    mysql_close(conn);
  }

  if (mysql_query(conn, "SELECT RestaurantName ,MaxOilCount, CurrentOilCount , Coefficient, Frequency FROM RESTAURANTS"))
  {
    centeredText("\033[31m Query execution error.");
    mysql_close(conn);
  }

  MYSQL_RES *result = mysql_store_result(conn);
  if (result == nullptr)
  {
    centeredText("\033[31m Result fetching error.");
    mysql_close(conn);
    return;
  }

  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;

  std::cout << "\t\t\t\t\t+---------------+-------------+-----------------+-------------+-----------+" << std::endl;
  std::cout << "\t\t\t\t\t|RestaurantName | MaxOilCount | CurrentOilCount | Coefficient | Frequency |" << std::endl;
  std::cout << "\t\t\t\t\t+---------------+-------------+-----------------+-------------+-----------+" << std::endl;
  while ((row = mysql_fetch_row(result)))
  {
    for (int i = 0; i < num_fields; ++i)
    {
      if (row[i] != nullptr)
      {
        if (i == 0)
        {
          std::cout << "\t\t\t\t\t|";
          std::size_t length = std::strlen(row[i]);
          std::cout << row[i];
          space(14 - length);
          std::cout << "|";
        }
        else if (i % 4 == 1)
        {

          std::size_t length = std::strlen(row[i]);
          std::cout << row[i];
          space(12 - length);
          std::cout << "|";
        }
        else if (i % 4 == 2)
        {

          std::size_t length = std::strlen(row[i]);
          std::cout << row[i];
          space(16 - length);
          std::cout << "|";
        }
        else if (i % 4 == 3)
        {

          std::size_t length = std::strlen(row[i]);
          std::cout << row[i];
          space(12 - length);
          std::cout << "|";
        }
        else

            if (i % 4 == 0)
        {
          std::size_t length = std::strlen(row[i]);
          std::cout << row[i];
          space(10 - length);
          std::cout << "|";
          std::cout << std::endl;
          std::cout << "\t\t\t\t\t+---------------+-------------+-----------------+-------------+-----------+" << std::endl;
        }
      }
    }
  }
  mysql_free_result(result);
  mysql_close(conn);
}
void SQL::alignment(string &passWord, string &adminName)
{
  passWord = password;
  adminName = adminname;
}
Admin::Admin() {}

void Admin::loadingAnimation()
{
  int totalIterations = 100;

  for (int i = 0; i <= totalIterations; ++i)
  {
    float progress = static_cast<float>(i) / totalIterations;

    header();

    cout << "Loading: [ ";

    int barWidth = getTerminalWidth() - 20;
    int progressWidth = static_cast<int>(barWidth * progress);

    for (int j = 0; j < barWidth; ++j)
    {
      if (j <= progressWidth)
      {
        cout << "#";
      }
      else
      {
        cout << ".";
      }
    }

    cout << " ] " << int(progress * 100.0) << "%" << endl;

    this_thread::sleep_for(chrono::milliseconds(50));
  }

  centeredText("Loading complete!");
}

void Admin::adminLogin()
{

  SQL obj;
  Admin obj2;

  string adminName;
  string adminPass;

  obj.SQLgetAdminPassLogin();
  obj.alignment(adminPass, adminName);
  Admin admin(adminPass, adminName);

  sleep(1);
  header();

  centeredText("Welcome to the Admin Login Portal");

  string enteredAdminName;
  string enteredAdminPass;
  Admin::enterPassLog(enteredAdminPass, enteredAdminName);
  int attempt = 0;
  if (admin.login(enteredAdminPass, enteredAdminName))
  {
    header();
    centeredText("Login successful!");
    admin.adminMainPage();
  }
  else
  {
    while (attempt != 3)
    {

      header();
      attempt++;
      centeredText("\033[33m Incorrect Password Attempt ");
      centeredText("Try again !!");
      std::cout<< "   ( "<< attempt << " of 3 )"<<std::endl ; 
     

      Admin::enterPassLog(enteredAdminPass, enteredAdminName);
      if (admin.login(enteredAdminPass, enteredAdminName))
      {
        header();
        centeredText("\033[32m Login successful!");
        admin.adminMainPage();
        break;
      }
      else if (attempt == 3)
      {

        system("clear");
        centeredText("\033[31m Too Many Incorrect Password Attempts:");
        sleep(1);
        exit(1);
      }
    }
  }
}
void Admin::enterPassLog(string &enteredPass, string &enteredName)
{

  cout << "Admin Username: ";
  cin >> enteredName;

  cout << "Admin Password: ";
  cin >> enteredPass;
}
void AdminPage()
{
  Admin obj;
  obj.checkingConnSQL();
}

void Admin::adminMainPage()
{
  SQL sql;
  header();
  centeredText("Main Page");
  sql.SQLgetData();
}

int main()
{
  AdminPage();
  return 0;
}