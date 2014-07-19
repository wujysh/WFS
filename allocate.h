#ifndef _ALLOCATE_H
#define _ALLOCATE_H

#include "print.h"

int allocateInode() {
    int index = -1;
    if (!idle_inode_stack.empty()) {
        index = idle_inode_stack.back();
        idle_inode_stack.pop_back();
    } else {
        cout << "WFS: FATAL ERROR: no available Inode" << endl;
    }
    writeIdleInode();
    return index;
}

int allocateBlock() {
    int block_index = -1;
    if (!idle_block_stack.empty()) {
        block_index = idle_block_stack.back();
        idle_block_stack.pop_back();
    } else {
        // grouped chain
        if (idle_block_list_index < 7) {
            popIdleBlockList();
            block_index = idle_block_stack.back();
            idle_block_stack.pop_back();
        } else {
            cout << "WFS: FATAL ERROR: no available block" << endl;
        }
    }
    //printIdleBlockStack();
    return block_index;
}

void releaseInode(int index) {
    idle_inode_stack.push_back(index);
    inodes.erase(index);
    writeIdleInode();
}

void releaseBlock(int block_index) {
    if (idle_block_stack.size() >= 132) {
        // grouped chain
        pushIdleBlockList();
        idle_block_stack.clear();
    }
    idle_block_stack.push_back(block_index);
    //printIdleBlockStack();
}

#endif // _ALLOCATE_H
