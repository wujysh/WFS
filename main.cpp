/**
 * Wujy File System (WFS)
 * Version 0.1.6
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
    clear_screen();
}

void saveSystem() {
    cout << "Saving changes ..." << endl;
    writeIdleInode();
    writeIdleBlock();
    //writeInodeAll();
    clear_screen();
}

int main() {
    readSetting(formatted);

    if (!formatted) {
        cout << "The disk hasn't formatted. Please wait a second ..." << endl;
        if (format()) {
            clear_screen();
            cout << "Formatted successfully." << endl;
        } else {
            clear_screen();
            cout << "Something wrong. Please try again." << endl;
        };
        cout << endl;
    }

    if (formatted) {
        initSystem();

        switchUser();
        initPath();
        /// printMenu();

        readCommand();

        saveSystem();

        cout << "Good bye!" << endl;
    }

    return 0;
}
