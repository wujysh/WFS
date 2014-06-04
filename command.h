#ifndef _PRINT_H_
#define _PRINT_H_
#include "print.h"
#endif // _PRINT_H_

#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_
#include "directory.h"
#endif // _DIRECTORY_H_

vector<string> getOptions(int n) {
    string line, op;
    vector<string> ret;
    getline(cin, line);
    stringstream ss(line);
    while (ss >> op) {
        ret.push_back(op);
    }
    return ret;
}

void readCommand() {
    string command;
    vector<string> options;
    while (true) {
        printPath();

        cin >> command;
        transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (command == "dir" || command == "ls") {
            options = getOptions(1);
            if (options.size() == 1 && options[0] == "-l") {
                ls(true);  // show details
            } else {
                ls(false);
            }
        } else if (command == "cd") {
            options = getOptions(1);
            if (options.size() == 1) {
                cd(options[0]);
            } else {
                cout << "cd: usage: cd [dir]" << endl;
            }
        } else if (command == "mkdir") {
            options = getOptions(1);
            if (options.size() == 1) {
                mkdir(options[0]);
            } else {
                cout << "mkdir: usage: mkdir [dir]" << endl;
            }
        } else if (command == "rmdir") {
            options = getOptions(1);
            if (options.size() == 1) {
                rmdir(options[0]);
            } else {
                cout << "rmdir: usage: rmdir [dir]" << endl;
            }
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
            options = getOptions(2);
            if (options.size() == 2) {
                if (options[0] == "-i") {
                    printInode(atoi(options[1].c_str()));
                } else if (options[0] == "-d") {
                    printDirectory(atoi(options[1].c_str()));
                } else {
                    cout << "debug: usage: debug [-i|-d] [index]" << endl;
                }
            } else if (options.size() == 1) {
                // TODO: print Block info
            } else {
                cout << "debug: usage: debug [-i|-d] [index]" << endl;
            }
        } else {
            cout << command << ": command not found" << endl;
        }
    }
}
