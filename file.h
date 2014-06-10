#ifndef _AUTHORITY_H_
#define _AUTHORITY_H_
#include "authority.h"
#endif // _AUTHORITY_H_

void mkfile(string name) {
    int index = path.back().inode;
    Inode inode = getInode(index);

    if (!canWrite(index)) {
        cout << "mk: " << name << ": No authority" << endl;
        return;
    }

    map<string, int> directory = getDirectory(index);

    if (directory.find(name) != directory.end()) {
        cout << "mk: cannot create file '" << name << "': File exists" << endl;
        return;
    }

    int childIndex = allocateInode();
    Inode childInode = Inode(childIndex, "-rwxr-xr-x", users[username].id, users[username].gid, 0, 1);
    childInode.addr[0] = allocateBlock();
    inodes[childIndex] = childInode;

    directory[name] = childIndex;
    directories[index] = directory;
    //writeDirectory(childIndex);
    writeDirectory(index);

    inodes[childIndex] = childInode;
    writeInodeOneBlock(childInode, childIndex);
    writeInodeOneBlock(inode, index);
}

void rmfile(string name) {
    int index = path.back().inode;
    Inode inode = getInode(index);

    if (!canWrite(index)) {
        cout << "rm: " << name << ": No authority" << endl;
        return;
    }

    map<string, int> directory = getDirectory(index);

    if (directory.find(name) == directory.end()) {
        cout << "rm: failed to remove '"<< name << "': No such file or directory" << endl;
        return;
    }

    int childIndex = directory[name];
    Inode childInode = inodes[childIndex];

    if (!canWrite(childIndex)) {
        cout << "rm: " << name << ": No authority" << endl;
        return;
    }

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

    writeDirectory(index);
    writeInodeOneBlock(Inode(), childIndex);
}
