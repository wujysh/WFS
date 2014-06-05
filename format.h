#ifndef _BASE_H_
#define _BASE_H_
#include "base.h"
#endif // _BASE_H_

bool format() {
    formatted = true;
    writeSetting(formatted);

    users.clear();
    users["wujy"] = User("wujy", 0, 0, 1);
    users["admin"] = User("admin", 1, 1, 1);
    users["test"] = User("test", 2, 2, 0);
    writeUser();

    idle_inode_stack.clear();
    for (int i = 999; i > 3; i--) {
        idle_inode_stack.push_back(i);
    }
    writeIdleInode();

    idle_block_stack.clear();
    for (int i = 49; i > 3; i--) {
        idle_block_stack.push_back(i);
    }
    writeIdleBlock();

    inodes.clear();
    vector<int> addr(10, -1);
    addr[0] = 0;
    inodes[0] = Inode("dr-xr-x--x", 0, 0, 4096, 1, addr, -1);  // root (MFD)
    for (int i = 1; i <= 3; i++) {
        addr[0] = i;
        inodes[i] = Inode("drwx--x--x", i-1, i-1, 4096, 1, addr, -1);  // three default user (UFD)
    }
    addr[0] = -1;
    for (int i = 4; i < 5760; i++) {
        inodes[i] = Inode();  // empty inodes
    }
    writeInodeAll();

    directories.clear();
    directories[0]["."] = 0;
    directories[0]["wujy"] = 1;
    directories[0]["admin"] = 2;
    directories[0]["test"] = 3;
    directories[1]["."] = 1;
    directories[1][".."] = 0;
    directories[2]["."] = 2;
    directories[2][".."] = 0;
    directories[3]["."] = 3;
    directories[3][".."] = 0;
    writeDirectory(0);
    writeDirectory(1);
    writeDirectory(2);
    writeDirectory(3);

    return true;
}
