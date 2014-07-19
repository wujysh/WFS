#ifndef _FORMAT_H
#define _FORMAT_H

#include "base.h"
#include "common.h"

bool format() {
    formatted = true;
    idle_block_list_index = 0;
    writeSetting();

    users.clear();
    users["root"] = User("root", 0, 0, 1);
    users["wujy"] = User("wujy", 1, 0, 1);
    users["admin"] = User("admin", 2, 1, 1);
    users["test"] = User("test", 3, 2, 0);
    writeUser();

    idle_inode_stack.clear();
    for (int i = 999; i > 3; i--) {
        idle_inode_stack.push_back(i);
    }
    writeIdleInode();

    idle_block_list_index = 7;
    int i = 924;
    while (i > 4) {
        idle_block_stack.clear();
        for (int j = 0; j < 132; j++) {
            idle_block_stack.push_back(--i);
            if (i <= 4) break;
        }
        pushIdleBlockList();  // idle_block_list_index--;
    }

    inodes.clear();
    vector<int> addr(10, -1);
    addr[0] = 0;
    inodes[0] = Inode(0, "dr-xr-x--x", 0, 0, 4096, 1, addr, -1);  // root (MFD)
    for (int i = 1; i <= 3; i++) {
        addr[0] = i;
        inodes[i] = Inode(i, "drwx--x--x", i, i-1, 4096, 1, addr, -1);  // three default user (UFD)
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

#endif // _FORMAT_H
