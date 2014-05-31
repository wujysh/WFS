/**
 * Wujy File System (WFS)
 * Version 0.1
 * 2014.5.31
 */
#include "common.h"
#include "format.h"
#include "user.h"
#include "command.h"

void initSystem() {
    cout << "Initializing ..." << endl;
    readSetting(formatted);
    system("cls");
}

int main() {
    initSystem();

    if (!formatted) {
        cout << "The disk hasn't formatted. Please wait a second ..." << endl;
        if (format()) {
            formatted = true;
            writeSetting(formatted);
            system("cls");
            cout << "Formatted successfully." << endl;
        } else {
            system("cls");
            cout << "Something wrong. Please try again." << endl;
        };
        cout << endl;
    }

    if (formatted) {
        /// readSuper();

        cout << "Please login the file system:" << endl;
        while (!login()) {
            system("cls");
            cout << "Invalid username or password! Please try again." << endl;
        }
        system("cls");
        cout << "Hello " << username << "! Welcome to Wujy File System." << endl;

        /// printMenu();

        readCommand();

        /// writeSuper();
    }

    return 0;
}
