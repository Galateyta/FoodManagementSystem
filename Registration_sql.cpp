#include "/usr/include/mysql/mysql.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <cctype>
//#include "utils/Utils.h"

using namespace std;

void  login();
void forgot();
void firstpage();
void firstpage_two();
void profile();
void registration();
bool lettersAndNumbers();
//string enterRestName();
//string enterAddress();
string generateID();
string RuserName, RPassword;

int  main()
{
	cout<<"\t\t\t ___________________________________ \n";
	cout<<"\t\t\t ***             WELCOME!        *** \n";
	cout<<"\t\t\t    ______________________________   \n";
	cout<<"\t\t\t     Used Oil Management Programm    \n\n";
firstpage();
}
void firstpage() {	
	int a;	
	cout<<"\t * Press 1 to Login    "<<endl;
	cout<<"\t * Press 2 to Registor "<<endl;
	cout<<"\t * Press 3 if you forgot your password "<<endl;
	cout<<"\t * Press 4 to EXIT \n\n";
	cout<<"\t Enter your  choise: ";
	cin>>a;
	cout<<endl;

	switch(a)
	{
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
			cout<<"\t\t\t Thank you! \n\n";
			break;
		default:
			cout<<"\x1B[2J\x1B[H";
			cout<<"\t\t\t Select  from the options given above \n";
			firstpage();
	}
}

void firstpage_two() {
        int a;
        cout<<"\t * Press 1 to Login    "<<endl;
        cout<<"\t * Press 2 to EXIT \n\n";
        cout<<"\t Enter your  choise: ";
        cin>>a;
        cout<<endl;

        switch(a)
        {
                case 1:
                        login();
                        break;
                case 2:
                        cout<<"\t\t\t Thank you! \n\n";
	      		break;
        default:
			cout<<"\x1B[2J\x1B[H";
			cout<<"\t\t\t Select  from the options given above \n";
			firstpage();
        }
}

void profile(){
	cout<<"\t\t\t\t --------- Hello!--------- \n\n";
	cout<<"\t\t\t\t  Here is your Profile \n\n\n"; }
// ___________________________________________________________ //

string enterRestName(){
cout<<"\t\t\t Enter your public food place name: \n";
cout<<"\t\t\t __ ";
cin.ignore();
getline(cin, RestName);
return RestName;
}

string enterAddress(){
cout<<"\t\t\t Enter address: \n";
cout<<"\t\t\t __ ";
cin.ignore();
getline(cin, Address);
return Address;
}

string generateID(){

    string ID;
	srand(time(0));
	int random=(rand()%10000);
	string str=to_string(random);
	ID=RuserName+str;
return ID;
}

bool lettersAndNumbers(string& RPassword){
bool hasLetters=false;
bool hasNumbers=false;
for(char c: RPassword){
    if(isalpha(c)){
        hasLetters=true;
    }
    else if(isdigit(c)){
        hasNumbers=true; }
    if(hasLetters && hasNumbers){
    break;
    }
}
return hasLetters && hasNumbers;
}
// _________________________________________________________________ //


void  registration()
{
	string RestName, Address, Email; 
    int a;
    char userrole[20]="User";
	cout<<"\x1B[2J\x1B[H";
        
	cout<<"\t\t\t Create a USERNAME: "<<endl;
    cout<<"\t\t\t _ ";
    cin>>RuserName;
// ************************ //
	
	char server[30] = "sql6.freesqldatabase.com";
        char username[25] = "sql6642308";
        char password[25] = "ZL9cx52cPH";
        char database[25] = "sql6642308";

    MYSQL* conn = mysql_init(NULL);

    if (conn == NULL) {
        printf("MySQL initialization failed");
        return;}

    if (mysql_real_connect(conn, server, username, password, database, 0, NULL, 0) == NULL) {
        printf("Unable to connect with MySQL server\n");
        mysql_close(conn);
        return;}

    if(mysql_query(conn, "SELECT * FROM USER")) {
	    cout<<"Unable to  connect with MySQL server\n";
	    mysql_close(conn);
	    return;}
	
	MYSQL_RES *result = mysql_store_result(conn);
    	if (result == NULL) {
	    cout << "Result fetching error." <<endl;
        mysql_close(conn);
        return;}

	 int num_fields = mysql_num_fields(result);
         MYSQL_ROW row;
    	 while ((row = mysql_fetch_row(result))) {
        	for (int i = 0; i < num_fields; ++i) {
            	if (RuserName==row[i]) {
                cout << "\t\t\t This username already exist! Try again! "<<endl;
	            cout<<"\t\t\t __ ";
		        cin>>RuserName;
            	}
            }   
        }

    mysql_free_result(result);
    mysql_close(conn);
        	
// ************************* //
	cout<<"\n\n\t\t\t Enter your public food place name: "<<endl;
    cout<<"\t\t\t _ ";
    cin.ignore();
    getline(cin, RestName);
    //enterRestName();

    cout<<"\n\n\t\t\t Create a PASSWORD: "<<endl;
    cout<<"\t\t\t Your  password must  have letters, numbers and minimum 6 symbols "<< endl ;
    cout<<"\t\t\t _ ";
    cin>>RPassword;


        while  (RPassword.length() < 6 ) {
        cout<< "\t\t\t Your password must have minimum 6 symbols . Try again "<< endl ;
        cout<<"\t\t\t Create a PASSWORD: ";
        cout<<"\t\t\t _ ";
        cin >> RPassword ;
        }
        while(true){
            if(lettersAndNumbers(RPassword)){
            cout<<"\t\t\t Strong password! \n\n\n "; 
            break; }
            else{
            cout<<"\t\t\t Password have to  contain both  letters and numbers. \n";
            cout<<"\t\t\t __ ";
            cin>>RPassword;}
        }

    cout<<"\n\n\t\t\t Enter your address: "<<endl;
    cout<<"\t\t\t _ ";
    getline(cin, Address);
    cin.ignore();
    //enterAddress();
    
    cout<<"\n\n\t\t\t Enter your email: "<<endl;
    cout<<"\t\t\t _ ";
    cin>>Email;
// _________________________________________ //

	cout<<"\x1B[2J\x1B[H";
	cout<<"\n\t\t\t Your registration is done! \n";

    MYSQL* connn = mysql_init(NULL);

    if (connn == NULL) {
        printf("MySQL initialization failed");
        return;
    }

    if (mysql_real_connect(connn, server, username, password, database, 0, NULL, 0) == NULL) {
        printf("Unable to connect with MySQL server\n");
        mysql_close(connn);
        return;   
    }

  /*  if (mysql_query(conn, "CREATE TABLE USER(Eid INT PRIMARY KEY AUTO_INCREMENT, Ename VARCHAR(16), Salary INT)")) {
        printf("Unable to create database table in MyDb database\n");
        mysql_close(conn);
        return 1;
    }*/
    string userID=generateID();
    //string userID = generateUniqueID();

    string queryString = "INSERT INTO USER(ID, Username, RestName, Password, Address, Email, MenuID, UserRole) VALUES ('" + userID +  "','" + RuserName + "','" + RestName +"','" + RPassword + "','" + Address + "','" + Email + "','0','" + userrole + "')";
    string insertQuery = queryString;
    if (mysql_query(connn, insertQuery.c_str())) {
        mysql_close(connn);
        return;
    } 

		cout<<"\t\t\t Now you  can login to your profile. \n\n\n";
            firstpage();

}

// ________________________________________________________ //


void login() {
	int count;
	string UserName, Password;
        	
	cout<<"\x1B[2J\x1B[H";

// *************************************** //
	char server[30] = "sql6.freesqldatabase.com";
        char username[25] = "sql6642308";
        char password[25] = "ZL9cx52cPH";
        char database[25] = "sql6642308";

    	MYSQL* conn = mysql_init(NULL);

    if (conn == NULL) {
        printf("MySQL initialization failed");
        return;
    }

    if (mysql_real_connect(conn, server, username, password, database, 0, NULL, 0) == NULL) {
        printf("Unable to connect with MySQL server\n");
        mysql_close(conn);
        return;
    }

    bool exist = false;
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row;
    while(!exist) {
        cout<<"\t\t\t Enter username: \n";
        cout<<"\t\t\t __ ";
        cin >> UserName;
        cout<<"\t\t\t Enter password: \n";
        cout<<"\t\t\t __ ";
        cin >> Password;
        string selectAllQuery = "SELECT * FROM USER";

    if(mysql_query(conn, "SELECT * FROM USER")) {
            cout<<"Unable to  connect with MySQL server\n";
            mysql_close(conn);
            return;}
	
	MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
         cout << "Result fetching error." <<endl;
       	mysql_close(conn);
        return;
        }
 while ((row = mysql_fetch_row(result))) {
            if (UserName == row[1] && Password == row[3]) {
                exist = true;
                cout <<"\n\n\t\t\t\t Welcome to your profile! \n\n\n";
                profile();
                break;
            }
            else {
                exist = false;
            }
        }
        if(exist) {
            mysql_free_result(result);
            mysql_close(conn);
            break;
        }
        else {
            std::cout <<"Username or password is incorrect!. Try again! \n\n";
            continue;
        }
    }
    //mysql_free_result(result);
    //mysql_close(conn);
}
// ******************************************* //
	
// _______________________________________________ //

void forgot()
{
	int version;
	cout<<"\x1B[2J\x1B[H";
	cout<<"\n\n\n\t\t\t Forgot your password? We can help you!\n\n";
	cout<<"\t\t * Press 1 to search your ID by email"<<endl;
    cout<<"\t\t * Press 2 to  go to the main page"<<endl;
	cout<<"\t\t Enter your choise: \n";
    cout<<"\t\t __ ";
	cin>>version;

	switch(version)
	{
		case 1: 
			{
			int count=0;
			string Fuserid, Fid, Fpass;
			cout<<"\t\t\t Enter the USERNAME: ";
			cin>>Fuserid;
			ifstream f3("data.xls");
				if(f3.is_open()){
				while(f3>>Fid>>Fpass) {
					if(Fid==Fuserid){
						count=1; }
				}
			f3.close();}
			if(count==1){
				cout<<"\t\t Your account is found\n";
				cout<<"\t\t Your  password is: "<<Fpass;
				cout<<"\n\n\t\t\t Now you  can login to your profile \n\n";
				firstpage(); }
			else {
				cout<<"\n\t\t Your  account is not  found!\n";
				firstpage();
			}
			break; }
		case 2:
			{
				main(); }
		default:
			cout<<"\t\t Wrong choice! Try  again!";
	}
}


