void printInode(int index) {
    Inode inode = getInode(index);

    cout << inode.mode << endl;
    cout << inode.uid << endl;
    cout << inode.gid << endl;
    cout << inode.file_size << endl;
    cout << inode.block_cnt << endl;
    for (int i = 0; i < 10; i++) {
        if (i) cout << " ";
        cout << inode.addr[i];
    }
    cout << endl;
    cout << inode.addr1 << endl;
}

void printDirectory(int index, bool showHide) {
    map<string, int> directory = getDirectory(index);
    map<string, int>::iterator it;

    bool isPrinted = false;
    cout.flags(ios::left);
    for (it = directory.begin(); it != directory.end(); it++) {
        if (showHide || it->first[0] != '.') {
            isPrinted = true;
            cout << setw(20) << it->first;
        }
    }
    if (isPrinted) cout << endl;
}

void printDirectoryDetail(int index, bool showHide) {
    map<string, int> directory = getDirectory(index);
    map<string, int>::iterator it;

    for (it = directory.begin(); it != directory.end(); it++) {
        if (showHide || it->first[0] != '.') {
            Inode inode = getInode(it->second);
            cout << it->second << " " << inode.mode << " "  << name_of_user[inode.uid] << " "  << name_of_group[inode.gid] << " "  << inode.file_size << " "  << it->first << endl;
        }
    }
}

void printPath() {
    cout << endl;
    for (int i = 1; i < path.size(); i++) {
        cout << '/' << path[i].name;
    }
    cout << "> ";
}

void printUsage(string command, string usage) {
    cout << command << ": usage: " << command << " " << usage << endl;
}

void printHelp(string command, string usage, string function) {
    cout.flags(ios::left);
    cout << setw(10) << command << setw(10) << "usage: " << command << " " << usage << endl;
    cout << setw(10) << "" << setw(10) << "function: " << function << endl;
}

void printMenu() {
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
}

void printOpenFile() {
    list<int>::iterator it;
    list<int> userOpenFiles = users[username].openFiles;

    cout << "Open files: " << endl;
    for (it = userOpenFiles.begin(); it != userOpenFiles.end(); it++) {
        OpenFile o = openFiles[*it];
        cout << *it << " " << o.flag << " " << o.count << " " << o.index << " " << o.offset << endl;
    }
}
