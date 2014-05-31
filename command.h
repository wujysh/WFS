void printPath() {
    cout << endl;
    for (int i = 0; i < path.size(); i++) {
        cout << '/' << path[i];
    }
    cout << '>';
}

void readCommand() {
    string command;
    while (true) {
        printPath();

        cin >> command;
        if (command == "dir" || command == "ls") {

        } else if (command == "cd") {

        } else if (command == "create" || command == "mk") {

        } else if (command == "delete" || command == "rm") {

        } else if (command == "open") {

        } else if (command == "close") {

        } else if (command == "read") {

        } else if (command == "write") {

        } else if (command == "login") {

        } else if (command == "menu" || command == "help") {

        }
    }
}
