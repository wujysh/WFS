#ifndef _PRINT_H_
#define _PRINT_H_
#include "print.h"
#endif // _PRINT_H_

#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_
#include "directory.h"
#endif // _DIRECTORY_H_

void readCommand() {
    string command;
    while (true) {
        printPath();

        cin >> command;
        transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (command == "dir" || command == "ls") {
            ls();
        } else if (command == "cd") {
            string name;
            cin >> name;
            cd(name);
        } else if (command == "create" || command == "mk") {

        } else if (command == "delete" || command == "rm") {

        } else if (command == "open") {

        } else if (command == "close") {

        } else if (command == "read") {

        } else if (command == "write") {

        } else if (command == "login" || command == "logout") {
            switchUser();
        } else if (command == "menu" || command == "help") {

        } else if (command == "exit" || command == "quit") {
            break;
        } else if (command == "debug") {
            string op;
            int index;
            cin >> op;
            if (op == "-i") {
                cin >> index;
                printInode(index);
            } else if (op == "-d") {
                cin >> index;
                printDirectory(index);
            }
        }
    }
}
