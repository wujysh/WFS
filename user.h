#ifndef _USER_H
#define _USER_H

#include "base.h"
#include "directory.h"
#include "common.h"

void inputPassword(string &);
bool login();
void switchUser();
bool createUser();
bool deleteUser();


#ifdef __linux__
    void inputPassword(string &pwd) {
        cin >> pwd;
    }

#elif _WIN32
    #include <conio.h>

    void inputPassword(string &pwd) {
        char ch;
        while (true) {
            ch = getch();
            if (ch != char(13)) {
                switch (ch) {
                case 8:
                    if (pwd.length() != 0) {
                        cout << "\b \b";
                        pwd = pwd.substr(0, pwd.length()-1);
                    }
                    break;
                default:
                    cout << "*";
                    pwd += ch;
                    break;
                }
            } else {
                break;
            }
        }
    }
#endif

bool login() {
    //readUser();
    string name, pwd;
    cout << "Username: ";
    cin >> name;
    cout << "Password: ";
    inputPassword(pwd);
    if (users.find(name) != users.end() && users[name].password == pwd) {
        username = name;
        return true;
    } else {
        return false;
    }
}

void switchUser() {
    clearScreen();
    cout << "Please login the file system:" << endl;
    while (!login()) {
        clearScreen();
        cout << "Invalid username or password! Please try again." << endl;
    }
    clearScreen();

    path.clear();
    path.push_back(Path(0, ""));
    cd(username);

    cout << "Hello " << username << "! Welcome to Wujy File System." << endl;
}

bool createUser() {
    return true;
}

bool deleteUser() {
    return true;
}

#endif // _USER_H
