#ifndef _MEMORY_H
#define _MEMORY_H

#include "authority.h"

void open(string);
void close(int);
void read(int);
void write(int);


void open(string name) {
    // TODO: mode
    int index = path.back().inode;
    Inode inode = getInode(index);
    map<string, int> directory = getDirectory(index);

    if (directory.find(name) == directory.end()) {
        cout << "open: failed to open '"<< name << "': No such file or directory" << endl;
        return;
    }

    int childIndex = directory[name];
    Inode childInode = getInode(childIndex);

    if (!canRead(childIndex)) {
        cout << "open: failed to open '" << name << "': No authority" << endl;
        return;
    }

    if (childInode.mode[0] != '-') {
        cout << "open: failed to open '" << name << "': Not a file" << endl;
        return;
    }

    inode.open_cnt++;
    users[username].openFiles[users[username].openFiles.size()] = openFiles.size();
    openFiles.push_back(OpenFile(0, 0, childIndex, 0));

    printOpenFile();
}

void close(int descriptor) {
    if (users[username].openFiles.find(descriptor) == users[username].openFiles.end()) {
        cout << "close: failed to close '"<< descriptor << "': No such open file" << endl;
    }

    openFiles[users[username].openFiles[descriptor]].flag = 1;
    inodes[openFiles[users[username].openFiles[descriptor]].index].open_cnt--;
    users[username].openFiles.erase(descriptor);

    printOpenFile();
}

void read(int descriptor) {
    if (users[username].openFiles.find(descriptor) == users[username].openFiles.end()) {
        cout << "read: failed to read '"<< descriptor << "': No such open file" << endl;
        return;
    }

    string data;
    OpenFile file = openFiles[users[username].openFiles[descriptor]];
    readData(file.index, data);
    openFiles[users[username].openFiles[descriptor]].offset = data.size();

    cout << data << endl;
}

void write(int descriptor) {
    if (users[username].openFiles.find(descriptor) == users[username].openFiles.end()) {
        cout << "write: failed to write '"<< descriptor << "': No such open file" << endl;
        return;
    }

    cout << "Input the file content: (Press ':q' or 'exit' to save)" << endl;
    string data, line;
    while (getline(cin, line) && line != ":q" && line != "exit" && line != "quit") {
        data += line + "\n";
    }

    OpenFile file = openFiles[users[username].openFiles[descriptor]];
    inodes[file.index].file_size = data.size();
    writeData(file.index, data);
    openFiles[users[username].openFiles[descriptor]].offset = data.size();
}

#endif // _MEMORY_H
