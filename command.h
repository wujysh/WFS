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
            } else {
                printUsage(command, "[file]");
            }
        } else if (command == "close") {
            options = getOptions(1);
            if (options.size() == 1) {
                close(options[0]);
            } else {
                printUsage(command, "[file]");
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
        } else if (command == "login" || command == "logout") {
            switchUser();
        } else if (command == "menu" || command == "help" || command == "about") {
            cout << "WFS console (version 0.2.0) - Made by Jiaye Wu." << endl;
            cout << "You can use these commands. Type 'help' to see this list." << endl;
            printHelp("ls", "[-a|-l|-al]", "List information about the FILEs in current directory.");
            printHelp("cd", "[dir]", "Change the current working directory.");
            printHelp("mkdir", "[dir]", "Create the DIRECTORY, if it do not already exist.");
            printHelp("rmdir", "[dir]", "Remove the DIRECTORY, if it is empty.");
            printHelp("mk", "[file]", "Create the FILE, if it does not already exist.");
            printHelp("rm", "[file]", "Remove (unlink) the FILE.");  // TODO: link and unlink
            printHelp("open", "[file]", "Open the FILE, if it exists.");
            printHelp("close", "[file]", "Close the FILE, if it has already open.");
            printHelp("read", "[file]", "Read the FILE, and open it if hasn't.");
            printHelp("write", "[file]", "Write the FILE.");
            printHelp("format", "", "Reset the file system, and you will lose data. DANGEROUS!");
            printHelp("debug", "[-i|-d] [index]", "Output the information of Inode or Block to help debug.");
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
                    cout << "debug: usage: debug [-i|-d] [index]" << endl;
                }
            } else if (options.size() == 1) {
                // TODO: print Block info
            } else {
                cout << "debug: usage: debug [-i|-d] [index]" << endl;
            }
        } else {
            getOptions(1);
            cout << command << ": command not found" << endl;
        }
    }
}
