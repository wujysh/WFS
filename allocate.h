int allocateInode() {
    int index = -1;
    if (idle_inode_stack.size()) {
        index = idle_inode_stack.back();
        idle_inode_stack.pop_back();
    } else {
        cout << "WFS: FATAL ERROR: no available Inode" << endl;
    }
    //writeIdleInode();
    return index;
}

int allocateBlock() {
    int block_index = -1;
    if (idle_block_stack.size()) {
        block_index = idle_block_stack.back();
        idle_block_stack.pop_back();
    } else {
        // TODO: grouped chain
        cout << "WFS: FATAL ERROR: no available block" << endl;
    }
    //writeIdleBlock();
    return block_index;
}

void releaseInode(int index) {
    idle_inode_stack.push_back(index);
    inodes.erase(index);
    //writeIdleInode();
}

void releaseBlock(int block_index) {
    idle_block_stack.push_back(block_index);
    if (idle_block_stack.size() > 50) {
        // TODO: grouped chain
    }
    //writeIdleBlock();
}
