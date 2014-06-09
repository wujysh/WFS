void open(string name) {
    // TODO: mode
    int index = path.back().inode;
    Inode inode = getInode(index);
    map<string, int> directory = getDirectory(index);

    if (directory.find(name) == directory.end()) {
        cout << "open: failed to open '"<< name << "': No such file or directory" << endl;
        return;
    }

    int childIndex = directory[name];
    Inode childInode = inodes[childIndex];

    if (childInode.mode[0] != '-') {
        cout << "open: failed to open '" << name << "': Not a file" << endl;
        return;
    }

    inode.open_cnt++;
    //users[username].openFiles[openFiles.size()] = openFiles.size();
    users[username].openFiles.push_back(openFiles.size());
    openFiles.push_back(OpenFile(0, 0, childIndex, 0));

    printOpenFile();
}

void close(int descriptor) {

}

void read(string name) {

}

void write(string name) {

}
