#ifndef _PRINT_H_
#define _PRINT_H_
#include "print.h"
#endif // _PRINT_H_

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
            cout << "Invalid path!" << endl;
            return;
        }

        path.push_back(Path(directory[name], name));
    }
}

void ls() {
    printDirectory(path.back().inode);
}
