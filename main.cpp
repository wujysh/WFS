/**
 * Wujy File System (WFS)
 * Version 0.2.8
 * Date 2014.6.12
 */

#include "common.h"
#include "format.h"
#include "user.h"
#include "command.h"

void initSystem() {
    cout << "Initializing ..." << endl;
    readIdleInode();
    readIdleBlock();
    clearScreen();
}

void saveSystem() {
    cout << "Saving changes ..." << endl;
    writeIdleInode();
    writeIdleBlock();
    //writeInodeAll();
    clearScreen();
}

int main() {
    readSetting(formatted);
    readUser();

    if (!formatted) {
        cout << "The disk hasn't formatted. Please wait a second ..." << endl;
        if (format()) {
            clearScreen();
            cout << "Formatted successfully." << endl;
        } else {
            clearScreen();
            cout << "Something wrong. Please try again." << endl;
        };
        cout << endl;
    }

    if (formatted) {
        initSystem();
        switchUser();
        readCommand();
        saveSystem();
        cout << "Good bye!" << endl;
    }

    return 0;
}
