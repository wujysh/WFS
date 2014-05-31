#include "base.h"
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
    if (users.find(name) != users.end() && users[name] == pwd) {
        username = name;
        return true;
    } else {
        return false;
    }
}

bool logout() {
    return true;
}

bool createUser() {
    return true;
}

bool deleteUser() {
    return true;
}
