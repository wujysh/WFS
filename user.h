#ifndef _BASE_H_
#define _BASE_H_
#include "base.h"
#endif // _BASE_H_
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

bool login() {
    readUser();
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
    system("cls");
    cout << "Please login the file system:" << endl;
    while (!login()) {
        system("cls");
        cout << "Invalid username or password! Please try again." << endl;
    }
    system("cls");
    cout << "Hello " << username << "! Welcome to Wujy File System." << endl;
}

bool createUser() {
    return true;
}

bool deleteUser() {
    return true;
}
