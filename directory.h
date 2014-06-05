#ifndef _PRINT_H_
#define _PRINT_H_
#include "print.h"
#endif // _PRINT_H_

#include "allocate.h"

void cd(string name) {
    if (name == ".") {

    } else if (name == "..") {
        if (path.size() > 1) {
            path.pop_back();
        }
    } else {
        int index = path.back().inode;
        Inode inode = getInode(index);
        map<string, int> directory = getDirectory(index);

        if (directory.find(name) == directory.end()) {
            cout << "cd: " << name << ": No such file or directory" << endl;
            return;
        }

        if (getInode(directory[name]).mode[0] != 'd') {
            cout << "cd: " << name << ": Not a directory" << endl;
            return;
        }

        path.push_back(Path(directory[name], name));
    }
}

void ls(int type) {
    if (type == 0) {  // ls (basic)
        printDirectory(path.back().inode, false);
    } else if (type == 1) {  // ls -l
        printDirectoryDetail(path.back().inode, false);
    } else if (type == 2) {  // ls -al
        printDirectoryDetail(path.back().inode, true);
    } else if (type == 3) {  // ls -a
        printDirectory(path.back().inode, true);
    }
}

void mkdir(string name) {
    int index = path.back().inode;
    Inode inode = getInode(index);
    map<string, int> directory = getDirectory(index);

    if (directory.find(name) != directory.end()) {
        cout << "mkdir: cannot create directory '" << name << "': File exists" << endl;
        return;
    }

    int childIndex = allocateInode();
    Inode childInode = inodes[childIndex];

    childInode.mode = "drwxr-xr-x";
    childInode.uid = users[username].id;
    childInode.gid = users[username].gid;
    childInode.file_size = 4096;
    childInode.block_cnt = 1;
    childInode.addr[0] = allocateBlock();

    directory[name] = childIndex;
    directories[index] = directory;
    writeDirectory(childIndex);
    writeDirectory(index);

    inodes[childIndex] = childInode;
    writeInodeOneBlock(childInode, childIndex);
    writeInodeOneBlock(inode, index);
}

void rmdir(string name) {
    int index = path.back().inode;
    Inode inode = getInode(index);
    map<string, int> directory = getDirectory(index);

    if (directory.find(name) == directory.end()) {
        cout << "rmdir: failed to remove '"<< name << "': No such file or directory" << endl;
        return;
    }

    int childIndex = directory[name];
    Inode childInode = inodes[childIndex];

    if (childInode.mode[0] != 'd') {
        cout << "rmdir: failed to remove '" << name << "': Not a directory" << endl;
        return;
    }

    map<string, int> childDirectory = getDirectory(childIndex);

    if (!childDirectory.empty()) {
        cout << "rmdir: failed to remove '" << name << "': Directory not empty" << endl;
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
    writeDirectory(childIndex);
    writeDirectory(index);

    writeInodeOneBlock(Inode(), childIndex);
}
