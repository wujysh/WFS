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

vector<int> allocateBlock(int n) {
    vector<int> blocks;
    for (int i = 0; i < n; i++) {
        int block_index = allocateBlock();
        if (block_index == -1) break;
        blocks.push_back(block_index);
    }
    //printIdleBlockStack();
    return blocks;
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

void releaseBlock(vector<int> &blocks) {
    sort(blocks.begin(), blocks.end());
    for (int i = blocks.size()-1; i >= 0; i--) {
        releaseBlock(blocks[i]);
    }
    //printIdleBlockStack();
}

void release(Inode &inode) {
    vector<int> blocks;
    for (int i = 0; i < inode.block_cnt; i++) {
        if (i < 10) {
            blocks.push_back(inode.addr[i]);
        } else {
            // Indirect
        }
    }
    releaseBlock(blocks);
    releaseInode(inode.index);
    writeInodeOneBlock(Inode(), inode.index);
}

#endif // _ALLOCATE_H
