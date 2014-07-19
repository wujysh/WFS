/**
 * Wujy File System (WFS)
 * Version 0.3
 * Date 2014.7.19
 */

#include "common.h"
#include "format.h"
#include "user.h"
#include "command.h"

void initSystem() {
    cout << "Initializing ..." << endl;
    readIdleInode();
    popIdleBlockList();
    clearScreen();
}

void saveSystem() {
    cout << "Saving changes ..." << endl;
    writeIdleInode();
    pushIdleBlockList();
    writeSetting();
    //writeInodeAll();
    clearScreen();
}

int main() {
    readSetting();
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
