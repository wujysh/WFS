// Block 0#
void readSetting(bool &formatted) {
    setting.open("./disk/super/0.disk", ios::in);
    setting >> formatted;
    setting.close();
}

void writeSetting(bool &formatted) {
    setting.open("./disk/super/0.disk", ios::out);
    setting << formatted << endl;
    setting.close();
}

// Block 1#
void readUser() {
    user.open("./disk/super/1.disk", ios::in);
    users.clear();
    string name, pwd;
    int uid, gid, role;
    while (user >> name >> pwd >> uid >> gid >> role) {
        users[name] = User(pwd, uid, gid, role);
        name_of_user[uid] = name;
        name_of_group[gid] = name;  // TODO: add group
    }
    user.close();
}

void writeUser() {
    user.open("./disk/super/1.disk", ios::out);
    map<string, User>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        User u = it->second;
        user << it->first << " " << u.password << " " << u.id << " " << u.gid << " " << u.role << endl;
    }
    user.close();
}

// Block 2#
void readIdleInode() {
    block.open("./disk/super/2.disk", ios::in);
    idle_inode_stack.clear();
    int x;
    while (block >> x) {
        idle_inode_stack.push_back(x);
    }
    block.close();
}

void writeIdleInode() {
    block.open("./disk/super/2.disk", ios::out);
    for (unsigned i = 0; i < idle_inode_stack.size(); i++) {
        if (i) block << ' ';
        block << idle_inode_stack[i];
    }
    block << endl;
    block.close();
}

// Block 3#
void readIdleBlock() {
    block.open("./disk/super/3.disk", ios::in);
    idle_block_stack.clear();
    int x;
    while (block >> x) {
        idle_block_stack.push_back(x);
    }
    block.close();
}

void writeIdleBlock() {
    block.open("./disk/super/3.disk", ios::out);
    for (unsigned i = 0; i < idle_block_stack.size(); i++) {
        if (i) block << ' ';
        block << idle_block_stack[i];
    }
    block << endl;
    block.close();
}

// Block 10# ~ 19#
string getInodePath(int index) {
    int blockNum = index / 64 + 10;
    stringstream ss;
    ss << blockNum;
    string blockNumStr;
    ss >> blockNumStr;
    string blockPath = "./disk/inode/" + blockNumStr + ".disk";
    return blockPath;
}

//void readInode(int index) {
//    block.open(getInodePath(index).c_str(), ios::in);
//
//    // block.seekg();
//    int offset = index % 64;
//    for (int i = 0; i < offset; i++) {
//        for (int j = 0; j < 8; j++) {
//            string line;
//            getline(block, line);
//        }
//    }
//
//    Inode inode;
//    block >> inode.mode;
//    block >> inode.uid;
//    block >> inode.gid;
//    block >> inode.file_size;
//    block >> inode.block_cnt;
//    for (int i = 0; i < 10; i++) {
//        block >> inode.addr[i];
//    }
//    block >> inode.addr1;
//    inodes[index] = inode;
//
//    block.close();
//}

void readInodeOneBlock(int index) {
    block.open(getInodePath(index).c_str(), ios::in);

    int offset = index / 64 * 64;
    for (int i = offset; i < offset+64; i++) {
        Inode inode;
        block >> inode.mode;
        block >> inode.link_cnt;
        block >> inode.uid;
        block >> inode.gid;
        block >> inode.file_size;
        block >> inode.block_cnt;
        for (int j = 0; j < 10; j++) {
            block >> inode.addr[j];
        }
        block >> inode.addr1;
        inodes[i] = inode;
    }

    block.close();
}

void writeInodeOneBlock(Inode inode, int index) {
    block.open(getInodePath(index).c_str(), ios::out);

    inodes[index] = inode;
    int offset = index / 64 * 64;
    for (int i = offset; i < offset+64; i++) {
        Inode node = inodes[i];
        block << node.mode << endl;
        block << node.link_cnt << endl;
        block << node.uid << endl;
        block << node.gid << endl;
        block << node.file_size << endl;
        block << node.block_cnt << endl;
        if (node.mode[0] == 'd') {
            node.file_size = node.block_cnt * 4096;
        }
        for (int j = 0; j < 10; j++) {
            if (j) block << " ";
            block << node.addr[j];
        }
        block << endl;
        block << node.addr1 << endl;
        //block << endl;
    }

    block.close();
}

void writeInodeAll() {
    int cnt = 0;
    map<int, Inode>::iterator it;
    block.open(getInodePath(0).c_str(), ios::out);
    for (it = inodes.begin(); it != inodes.end(); it++) {
        Inode inode = it->second;
        block << inode.mode << endl;
        block << inode.link_cnt << endl;
        block << inode.uid << endl;
        block << inode.gid << endl;
        block << inode.file_size << endl;
        block << inode.block_cnt << endl;
        for (int i = 0; i < 10; i++) {
            if (i) block << " ";
            block << inode.addr[i];
        }
        block << endl;
        block << inode.addr1 << endl;
        //block << endl;

        if (++cnt % 64 == 0 && cnt / 64 != 90) {
            // switch block
            block.close();
            block.open(getInodePath(cnt).c_str(), ios::out);
        }
    }
    block.close();
}

Inode getInode(int index) {
    if (inodes.find(index) == inodes.end()) {
        readInodeOneBlock(index);
    }
    return inodes[index];
}

// Block 100# ~ 1023#
string getDataPath(int index) {
    int blockNum = index + 100;
    stringstream ss;
    ss << blockNum;
    string blockNumStr;
    ss >> blockNumStr;
    string blockPath = "./disk/data/" + blockNumStr + ".disk";
    return blockPath;
}

void readDirectory(int index, int block_index) {
    block.open(getDataPath(block_index).c_str(), ios::in);

    string name;
    int inode;
    while (block >> name >> inode) {
        directories[index][name] = inode;
    }

    block.close();
}

void readDirectory(int index) {
    Inode inode = getInode(index);

    if (inode.mode[0] != 'd') {
        return;  // not directory
    }

    directories[index].clear();

    for (int i = 0; i < inode.block_cnt; i++) {
        if (i < 10) {
            readDirectory(index, inode.addr[i]);
        } else {
            // TODO: indirect
        }
    }
}

void writeDirectory(int index, int block_index, map<string, int>::iterator &it) {
    block.open(getDataPath(block_index).c_str(), ios::out);

    int cnt = 0;
    for ( ; cnt < 100 && it != directories[index].end(); it++, cnt++) {
        block << it->first << " " << it->second << endl;
    }

    block.close();
}

void writeDirectory(int index) {
    Inode inode = getInode(index);

    if (inode.mode[0] != 'd') {
        return;  // not directory
    }

    map<string, int>::iterator it = directories[index].begin();

    for (int i = 0; i < inode.block_cnt; i++) {
        if (i < 10) {
            writeDirectory(index, inode.addr[i], it);
        } else {
            // TODO: indirect
        }
    }
}

map<string, int> getDirectory(int index) {
    if (directories.find(index) == directories.end()) {
        readDirectory(index);
    }
    return directories[index];
}

void readData(int index, string &data) {
    block.open(getDataPath(index).c_str(), ios::in);

    string line;
    data.clear();
    while (getline(block, line)) {
        data += "\n" + line;
    }
    data += "\n";

    block.close();
}

void writeData(int index, string &data) {
    block.open(getDataPath(index).c_str(), ios::out);

    block << data << endl;

    block.close();
}
