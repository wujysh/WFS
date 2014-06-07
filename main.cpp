/**
 * Wujy File System (WFS)
 * Version 0.2.5
 * Date 2014.6.8
 */
#ifndef _COMMON_H_
#define _COMMON_H_
#include "common.h"
#endif // _COMMON_H_

#ifndef _FORMAT_H_
#define _FORMAT_H_
#include "format.h"
#endif // _FORMAT_H_

#ifndef _USER_H_
#define _USER_H_
#include "user.h"
#endif // _USER_H_

#ifndef _COMMAND_H_
#define _COMMAND_H_
#include "command.h"
#endif // _COMMAND_H_

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
