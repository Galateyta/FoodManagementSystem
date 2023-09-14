#include "admin.h"
#include "SQL.h"

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

  cout << setw(padding + textWidth) << text;
}
void centeredTextEndl(const string &text)
{

  int terminalWidth = getTerminalWidth();

  int textWidth = text.length();
  int padding = (terminalWidth - textWidth) / 2;

  cout << setw(padding + textWidth) << text << endl;
}
void header()
{
  system("clear");
  centeredTextEndl("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  centeredTextEndl("FOOD MANAGEMENT ");
}
void Admin::checkingConnSQL()
{
  Admin obj1;
  SQL obj2;
  header();
  centeredTextEndl("Checking connection whit SQL databasa , please white ...");
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
    cout << "\t\t\t\t\t\tUnable to connect with MySQL server\n";
    mysql_close(conn);
    return false;
  }

  if (mysql_query(conn, "SELECT * FROM Admin"))
  {
    cout << "\t\t\t\t\tQuery execution error." << std::endl;
    mysql_close(conn);
    return false;
  }
  MYSQL_RES *result = mysql_store_result(conn);
  if (result == nullptr)
  {
    cout << "\t\t\t\t\tResult fetching error." << std::endl;
    mysql_close(conn);
    return false;
  }
  else
  {
    cout << ("\t\t\t\t\t\t\tYour connection is successful!!!\n");
    return true;
  }
};
void SQL::SQLgetAdminPassLogin()

{
  if (mysql_real_connect(conn, server, username, SQLpassword, database, 0, nullptr, 0) == nullptr)
  {
    std::cerr << "\t\t\t\t\tUnable to connect with MySQL server\n";
    return;
  }

  if (mysql_query(conn, "SELECT * FROM Admin"))
  {
    std::cerr << "\t\t\t\t\tQuery execution error." << std::endl;
    mysql_close(conn);
    return;
  }

  MYSQL_RES *result = mysql_store_result(conn);
  if (result == nullptr)
  {
    std::cerr << "\t\t\t\t\tResult fetching error." << std::endl;
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
void SQL::SQLgetOrder()

{
  
  if (mysql_real_connect(conn, server, username, SQLpassword, database, 0, nullptr, 0) == nullptr)
  {
    std::cerr << "\t\t\t\t\tUnable to connect with MySQL server\n";
    return;
  }

  if (mysql_query(conn, "SELECT * FROM ASA"))
  {
    std::cerr << "\t\t\t\t\tQuery execution error." << std::endl;
    mysql_close(conn);
    return;
  }

  MYSQL_RES *result = mysql_store_result(conn);
  if (result == nullptr)
  {
    std::cerr << "\t\t\t\t\tResult fetching error." << std::endl;
    mysql_close(conn);
    return;
  }

  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result)))
  {
    for (int i = 0; i < num_fields; ++i)
    {
      if (row[i] != nullptr )
      {
        if (i == 3 ){
        std::cout << row[i] <<"  "<< std::endl;
        
        } else {
          std::cout<< row[i] << "  "  ; 
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

    cout << "\t\t\t\t\t\t\tLoading: [ ";

    int barWidth = 40;
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

  centeredTextEndl("Loading complete!");
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

  centeredTextEndl("Welcome to the Admin Login Portal");

  string enteredAdminName;
  string enteredAdminPass;
  Admin::enterPassLog(enteredAdminPass, enteredAdminName);
  int attempt = 0;
  if (admin.login(enteredAdminPass, enteredAdminName))
  {
    header();
    centeredTextEndl("Login successful!");
    admin.adminMainPage();
  }
  else
  {
    while (attempt != 3)
    {

      header();
      attempt++;
      centeredText("Incorrect Password Attempt (");
      cout << attempt << " of 3):\n";
      centeredTextEndl("Try again !!");

      Admin::enterPassLog(enteredAdminPass, enteredAdminName);
      if (admin.login(enteredAdminPass, enteredAdminName))
      {
        header();
        centeredTextEndl("Login successful!");
        admin.adminMainPage();
        break;
      }
      else if (attempt == 3)
      {

        system("clear");
        centeredTextEndl("Too Many Incorrect Password Attempts:");
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
void Admin::date()
{

  string command = "date '+%Y-%m-%d %H:%M:%S'";
  string result = "";

  FILE *pipe = popen(command.c_str(), "r");

  char buffer[128];
  while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
  {
    result += buffer;
  }

  pclose(pipe);

  cout << result << endl;
}
void Admin::adminMainPage()
{
  SQL sql ; 
  header();
  centeredTextEndl("Main Page");
  sql.SQLgetOrder() ; 
  
}

int main()
{
  AdminPage();
  return 0;
}