#ifndef _DIRECTORY_H
#define _DIRECTORY_H

#include "print.h"
#include "allocate.h"
#include "common.h"
#include "authority.h"

void cd(string name) {
    if (name == ".") {

    } else if (name == "..") {
        if (path.size() > 1) {
            int index = path[path.size()-2].inode;

            if (!canExecute(index)) {
                cout << "cd: " << name << ": No authority" << endl;
                return;
            }

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

        if (!canExecute(directory[name])) {
            cout << "cd: " << name << ": No authority" << endl;
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
    if (!canRead(path.back().inode)) {
        cout << "ls: " << path.back().name << ": No authority" << endl;
        return;
    }

    if (type == 0) {  // ls
        printDirectory(path.back().inode, false);
        return;
    } else if (type == 3) {  // ls -a
        printDirectory(path.back().inode, true);
        return;
    }

    if (!canExecute(path.back().inode)) {
        cout << "ls: " << path.back().name << ": No authority" << endl;
        return;
    }

    if (type == 1) {  // ls -l
        printDirectoryDetail(path.back().inode, false);
        return;
    } else if (type == 2) {  // ls -al
        printDirectoryDetail(path.back().inode, true);
        return;
    }
}

//void ls(string name, bool showHide) {
//    int index = path.back().inode;
//    Inode inode = getInode(index);
//    map<string, int> directory = getDirectory(index), childDirectory;
//
//    if (directory.find(name) == directory.end()) {
//        cout << "ls: "<< name << ": No such file or directory" << endl;
//        return;
//    }
//
//    int childIndex = directory[name];
//
//    if (!canRead(childIndex)) {
//        cout << "ls: " << name << ": No authority" << endl;
//        return;
//    }
//
//    Inode childInode = inodes[childIndex];
//
//    if (childInode.mode[0] != 'd') {
//        cout << "ls: " << name << ": Not a directory" << endl;
//        return;
//    }
//
//    printDirectory(childIndex, false);
//}

void mkdir(string name) {
    int index = path.back().inode;
    Inode inode = getInode(index);

    if (!canWrite(index)) {
        cout << "mkdir: " << name << ": No authority" << endl;
        return;
    }

    map<string, int> directory = getDirectory(index), childDirectory;

    if (directory.find(name) != directory.end()) {
        cout << "mkdir: cannot create directory '" << name << "': File exists" << endl;
        return;
    }

    int childIndex = allocateInode();
    Inode childInode = Inode(childIndex, "drwxr-xr-x", users[username].id, users[username].gid, 4096, 1);
    childInode.addr[0] = allocateBlock();
    inodes[childIndex] = childInode;

    childDirectory["."] = childIndex;
    childDirectory[".."] = index;

    directory[name] = childIndex;

    directories[index] = directory;
    directories[childIndex] = childDirectory;
    writeDirectory(index);
    writeDirectory(childIndex);

    inodes[childIndex] = childInode;
    writeInodeOneBlock(childInode, childIndex);
    writeInodeOneBlock(inode, index);
}

void rmdir(string name) {
    int index = path.back().inode;
    Inode inode = getInode(index);

    if (!canWrite(index)) {
        cout << "rmdir: " << name << ": No authority" << endl;
        return;
    }

    map<string, int> directory = getDirectory(index);

    if (directory.find(name) == directory.end()) {
        cout << "rmdir: failed to remove '"<< name << "': No such file or directory" << endl;
        return;
    }

    int childIndex = directory[name];
    Inode childInode = inodes[childIndex];

    if (!canWrite(childIndex)) {
        cout << "rmdir: " << name << ": No authority" << endl;
        return;
    }

    if (childInode.mode[0] != 'd') {
        cout << "rmdir: failed to remove '" << name << "': Not a directory" << endl;
        return;
    }

    map<string, int> childDirectory = getDirectory(childIndex);

    if (childDirectory.size() > 2) {  // . and ..
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

void chmod(string name, string mode) {
    int index = path.back().inode;
    Inode inode = getInode(index);
    map<string, int> directory = getDirectory(index);

    if (directory.find(name) == directory.end()) {
        cout << "chmod: " << name << ": No such file or directory" << endl;
        return;
    }

    int childIndex = directory[name];
    Inode childInode = inodes[childIndex];

    if (!canWrite(childIndex)) {
        cout << "chmod: " << name << ": No authority" << endl;
        return;
    }

    childInode.mode = mode;

    inodes[childIndex] = childInode;
    writeInodeOneBlock(childInode, childIndex);
}

void rename(string name, string new_name) {
    int index = path.back().inode;
    Inode inode = getInode(index);
    map<string, int> directory = getDirectory(index);

    if (directory.find(name) == directory.end()) {
        cout << "rename: " << name << ": No such file or directory" << endl;
        return;
    }

    if (directory.find(new_name) != directory.end()) {
        cout << "rename: " << new_name << ": File or directory exists" << endl;
        return;
    }

    int childIndex = directory[name];

    if (!canWrite(childIndex)) {
        cout << "rename: " << name << ": No authority" << endl;
        return;
    }

    directory[new_name] = directory[name];
    directory.erase(name);

    directories[index] = directory;
    writeDirectory(index);
}

#endif // _DIRECTORY_H
