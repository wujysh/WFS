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
