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
    for (int i = 1098; i >= 100; i--) {
        idle_inode_stack.push_back(i);
    }
    writeIdleInode();

    idle_block_stack.clear();
    for (int i = 149; i >= 100; i--) {
        idle_block_stack.push_back(i);
    }
    writeIdleBlock();

    inodes.clear();
    vector<int> addr(10, -1);
    addr[0] = 0;
    inodes[0] = Inode("d--x--x--x", 0, 0, 4096, 1, addr, -1);  // root (MFD)
    for (int i = 1; i <= 3; i++) {
        addr[0] = i;
        inodes[i] = Inode("d--x--x--x", i-1, i-1, 4096, 1, addr, -1);  // three default user (UFD)
    }
    addr[0] = -1;
    for (int i = 4; i < 5760; i++) {
        inodes[i] = Inode("----------", -1, -1, 0, 0, addr, -1);  // empty inodes
    }
    writeInodeAll();

    return true;
}
