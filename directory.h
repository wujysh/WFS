#ifndef _PRINT_H_
#define _PRINT_H_
#include "print.h"
#endif // _PRINT_H_

#include "allocate.h"

void cd(string name) {
    if (name == "..") {
        if (path.size() > 1) {
            path.pop_back();
        }
    } else {
        int index = path.back().inode;
        Inode inode = getInode(index);
        map<string, int> directory = getDirectory(index);

        if (directory.find(name) == directory.end()) {
            cout << "cd: No such file or directory" << endl;
            return;
        }

        path.push_back(Path(directory[name], name));
    }
}

void ls(bool detail) {
    if (detail) {
        printDirectoryDetail(path.back().inode);
    } else {
        printDirectory(path.back().inode);
    }
}

void mkdir(string name) {
    int index = path.back().inode;
    Inode inode = getInode(index);
    map<string, int> directory = getDirectory(index);

    if (directory.find(name) != directory.end()) {
        cout << "mkdir: cannot create directory ‘" << name << "’: File exists" << endl;
        return;
    }

    int childIndex = allocateInode();
    Inode childInode = inodes[childIndex];

    childInode.mode = "drwxr-x--x";
    childInode.uid = users[username].id;
    childInode.gid = users[username].gid;
    childInode.file_size = 4096;
    childInode.block_cnt = 1;
    childInode.addr[0] = allocateBlock();

    directory[name] = childIndex;
    directories[index] = directory;

    //writeDirectory(index);
    //writeInodeOneBlock(childInode, childIndex);
    //writeInodeOneBlock(inode, index);
}

void rmdir(string name) {
    int index = path.back().inode;
    Inode inode = getInode(index);
    map<string, int> directory = getDirectory(index);

    if (directory.find(name) == directory.end()) {
        cout << "rmdir: failed to remove ‘"<< name << "’: No such file or directory" << endl;
        return;
    }


}
