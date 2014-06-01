/**
 * Wujy File System (WFS)
 * Version 0.1.5
 * 2014.6.1
 */
#include "common.h"
#include "format.h"
#include "user.h"
#include "command.h"

void initSystem() {
    cout << "Initializing ..." << endl;
    readIdleInode();
    readIdleBlock();
    system("cls");
}

void saveSystem() {
    cout << "Saving changes ..." << endl;
    writeIdleInode();
    writeIdleBlock();
    //writeInodeAll();
    system("cls");
}

int main() {
    readSetting(formatted);

    if (!formatted) {
        cout << "The disk hasn't formatted. Please wait a second ..." << endl;
        if (format()) {
            system("cls");
            cout << "Formatted successfully." << endl;
        } else {
            system("cls");
            cout << "Something wrong. Please try again." << endl;
        };
        cout << endl;
    }

    if (formatted) {
        initSystem();

        switchUser();

        /// printMenu();

        readCommand();

        saveSystem();

        cout << "Good bye!" << endl;
    }

    return 0;
}
