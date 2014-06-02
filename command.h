#include "print.h"
#include "directory.h"

void printPath() {
    cout << endl;
    for (int i = 1; i < path.size(); i++) {
        cout << '/' << path[i].name;
    }
    cout << "> ";
}

void initPath() {
    path.clear();
    path.push_back(Path(0, ""));
    cd(username);
}

void readCommand() {
    string command;
    while (true) {
        printPath();

        cin >> command;
        transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (command == "dir" || command == "ls") {

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
            initPath();
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
