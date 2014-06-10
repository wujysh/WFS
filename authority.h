#ifndef _COMMON_H_
#define _COMMON_H_
#include "common.h"
#endif // _COMMON_H_

#ifndef _BASE_H_
#define _BASE_H_
#include "base.h"
#endif // _BASE_H_

bool canRead(int index) {
    if (username == "root") return true;

    Inode inode = getInode(index);
    if (!((inode.mode[1] == 'r' && inode.uid == users[username].id) || (inode.mode[4] == 'r' && inode.gid == users[username].gid) || (inode.mode[7] == 'r'))) {
        return false;
    }
    return true;
}

bool canWrite(int index) {
    if (username == "root") return true;

    Inode inode = getInode(index);
    if (!((inode.mode[2] == 'w' && inode.uid == users[username].id) || (inode.mode[5] == 'w' && inode.gid == users[username].gid) || (inode.mode[8] == 'w'))) {
        return false;
    }
    return true;
}

bool canExecute(int index) {
    if (username == "root") return true;

    Inode inode = getInode(index);
    if (!((inode.mode[3] == 'x' && inode.uid == users[username].id) || (inode.mode[6] == 'x' && inode.gid == users[username].gid) || (inode.mode[9] == 'x'))) {
        return false;
    }
    return true;
}
