#ifndef _FILE_H
#define _FILE_H

#include "authority.h"

void mkfile(string);
void rmfile(string);


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
    if (childInode.addr[0] == -1) return;
    inodes[childIndex] = childInode;

    directory[name] = childIndex;
    directories[index] = directory;
    string data = "";
    writeData(childIndex, data);
    writeDirectory(index);
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

    release(childInode);

    directory.erase(name);
    directories[index] = directory;
    writeDirectory(index);
}

int found_cnt;
vector<Path> found_path;

void searchFileRecursive(int index, string name) {
    map<string, int> directory = getDirectory(index);
    map<string, int>::iterator it;
    for (it = directory.begin(); it != directory.end(); it++) {
        if (it->first[0] != '.') {
            Inode inode = getInode(it->second);
            if (it->first.find(name) != string::npos) {
                for (unsigned i = 1; i < found_path.size(); i++) {
                    cout << '/' << found_path[i].name;
                }
                cout << "/" << it->first << endl;
                found_cnt++;
            }
            if (inode.mode[0] == 'd') {
                found_path.push_back(Path(it->second, it->first));
                searchFileRecursive(it->second, name);
                found_path.pop_back();
            }
        }
    }
}

void file_search(string name) {
    found_cnt = 0;
    found_path = path;
    searchFileRecursive(path.back().inode, name);
    if (!found_cnt) {
        cout << "find: failed to find '" << name << "': No such file or directory." << endl;
    } else {
        cout << "find: succeed to find '" << name << "': " << found_cnt << " matches." << endl;
    }
}

#endif // _FILE_H
