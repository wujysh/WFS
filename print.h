void printInode(int index) {
    Inode inode = getInode(index);

    cout << "DUBUG Inode #" << index << ": " << endl;
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
    cout << inode.addr1 << endl << endl;
}

void printDirectory(int index) {
    map<string, int> directory = getDirectory(index);
    map<string, int>::iterator it;

    cout << "DUBUG Directory (Inode #" << index << "): " << endl;
    for (it = directory.begin(); it != directory.end(); it++) {
        cout << it->first << " " << it->second << endl;
    }
    cout << endl;

    block.close();
}
