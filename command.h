#ifndef _PRINT_H_
#define _PRINT_H_
#include "print.h"
#endif // _PRINT_H_

#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_
#include "directory.h"
#endif // _DIRECTORY_H_

#ifndef _FILE_H_
#define _FILE_H_
#include "file.h"
#endif // _FILE_H_

#ifndef _MEMORY_H_
#define _MEMORY_H_
#include "memory.h"
#endif // _MEMORY_H_

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
            if (options.size() == 1) {
                if (options[0] == "-l") {
                    ls(1);
                } else if (options[0] == "-al") {
                    ls(2);
                } else if (options[0] == "-a") {
                    ls(3);
                } else {
                    printUsage(command, "[-a|-l|-al]");
                }
            } else {
                if (command == "dir") {
                    ls(2);
                } else if (command == "ls") {
                    ls(0);
                }
            }
        } else if (command == "cd" || command == "chdir") {
            options = getOptions(1);
            if (options.size() == 1) {
                cd(options[0]);
            } else {
                printUsage(command, "[dir]");
            }
        } else if (command == "mkdir") {
            options = getOptions(1);
            if (options.size() == 1) {
                mkdir(options[0]);
            } else {
                printUsage(command, "[dir]");
            }
        } else if (command == "rmdir") {
            options = getOptions(1);
            if (options.size() == 1) {
                rmdir(options[0]);
            } else {
                printUsage(command, "[dir]");
            }
        } else if (command == "create" || command == "mk" || command == "touch") {
            options = getOptions(1);
            if (options.size() == 1) {
                mkfile(options[0]);
            } else {
                printUsage(command, "[file]");
            }
        } else if (command == "delete" || command == "rm") {
            options = getOptions(1);
            if (options.size() == 1) {
                rmfile(options[0]);
            } else {
                printUsage(command, "[file]");
            }
        } else if (command == "open") {
            options = getOptions(1);
            if (options.size() == 1) {
                open(options[0]);
            } else if (options.size() == 0) {
                printOpenFile();
            } else {
                printUsage(command, "[file]");
            }
        } else if (command == "close") {
            options = getOptions(1);
            if (options.size() == 1) {
                close(atoi(options[0].c_str()));
            } else {
                printUsage(command, "[file descriptor]");
            }
        } else if (command == "read") {
            options = getOptions(1);
            if (options.size() == 1) {
                read(options[0]);
            } else {
                printUsage(command, "[file]");
            }
        } else if (command == "write") {
            options = getOptions(1);
            if (options.size() == 1) {
                write(options[0]);
            } else {
                printUsage(command, "[file]");
            }
        } else if (command == "chmod") {
            options = getOptions(2);
            if (options.size() == 2) {
                chmod(options[0], options[1]);
            } else {
                printUsage(command, "[dir|file] [mode]");
            }
        } else if (command == "login" || command == "logout") {
            switchUser();
        } else if (command == "menu" || command == "help" || command == "about") {
            printMenu();
        } else if (command == "exit" || command == "quit") {
            break;
        } else if (command == "format" || command == "reset") {
            cout << "WFS: ALERT: UNRECOVERABLE operation! Are you sure? [y/n] ";
            options.resize(1);
            cin >> options[0];
            if (options[0] == "y") {
                format();
                cout << "Initializing ..." << endl;
                readIdleInode();
                readIdleBlock();
                clearScreen();
                cout << "Formatted successfully." << endl;
                switchUser();
            } else {
                cout << "Canceled." << endl;
            }
        } else if (command == "debug") {
            options = getOptions(2);
            if (options.size() == 2) {
                if (options[0] == "-i") {
                    printInode(atoi(options[1].c_str()));
                } else if (options[0] == "-d") {
                    printDirectoryDetail(atoi(options[1].c_str()), true);
                } else {
                    printUsage(command, "[-i|-d] [index]");
                }
            } else if (options.size() == 1) {
                // TODO: print Block info
            } else {
                printUsage(command, "[-i|-d] [index]");
            }
        } else {
            getOptions(1);
            cout << command << ": command not found" << endl;
        }
    }
}
