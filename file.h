void mkfile(string name) {
    int index = path.back().inode;
    Inode inode = getInode(index);
    map<string, int> directory = getDirectory(index);

    if (directory.find(name) != directory.end()) {
        cout << "mk: cannot create file '" << name << "': File exists" << endl;
        return;
    }

    int childIndex = allocateInode();
    Inode childInode = inodes[childIndex];

    childInode.mode = "-rwxr-xr-x";
    childInode.uid = users[username].id;
    childInode.gid = users[username].gid;
    childInode.file_size = 0;
    childInode.block_cnt = 1;
    childInode.addr[0] = allocateBlock();

    directory[name] = childIndex;
    directories[index] = directory;
    writeDirectory(childIndex);
    writeDirectory(index);

    inodes[childIndex] = childInode;
    writeInodeOneBlock(childInode, childIndex);
    writeInodeOneBlock(inode, index);
}

void rmfile(string name) {
    int index = path.back().inode;
    Inode inode = getInode(index);
    map<string, int> directory = getDirectory(index);

    if (directory.find(name) == directory.end()) {
        cout << "rm: failed to remove '"<< name << "': No such file or directory" << endl;
        return;
    }

    int childIndex = directory[name];
    Inode childInode = inodes[childIndex];

    if (childInode.mode[0] != '-') {
        cout << "rm: failed to remove '" << name << "': Not a file" << endl;
        return;
    }

    for (int i = 0; i < childInode.block_cnt; i++) {
        if (i < 10) {
            releaseBlock(childInode.addr[i]);
        } else {
            // Indirect
        }
    }
    releaseInode(childIndex);

    directory.erase(name);
    directories[index] = directory;
    writeDirectory(childIndex);
    writeDirectory(index);

    writeInodeOneBlock(Inode(), childIndex);
}

void open(string name) {

}

void close(string name) {

}

void read(string name) {

}

void write(string name) {

}
