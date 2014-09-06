#ifndef _BASE_H
#define _BASE_H

#include "common.h"

// Block 0#
void readSetting();
void writeSetting();

// Block 1#
void readUser();
void writeUser();

// Block 2#
void readIdleInode();
void writeIdleInode();

// Block 3# ~ 9#
string getIdleBlockPath(int);
void readIdleBlock(int);
void writeIdleBlock(int);
void popIdleBlockList();
void pushIdleBlockList();

// Block 10# ~ 19#
string getInodePath(int);
void readInodeOneBlock(int);
void writeInodeOneBlock(Inode, int);
void writeInodeAll();
Inode getInode(int);
int allocateInode();
int allocateBlock();
vector<int> allocateBlock(int);
void releaseInode(int);
void releaseBlock(int);
void releaseBlock(vector<int> &);
void release(Inode &);
void rebuildInode(Inode &, int);

// Block 100# ~ 1023#
string getDataPath(int);
void readIndirectBlock(int, vector<int> &);
void writeIndirectBlock(int, vector<int> &);
void readDirectory(int, int);
void readDirectory(int);
void writeDirectory(int, int, map<string, int>::iterator &);
void writeDirectory(int);
map<string, int> getDirectory(int);
void readData(int, int, string &);
void readData(int, string &);
void writeData(int, int, string &, string::iterator &);
void writeData(int, string &);


// Block 0#
void readSetting() {
    setting.open("./disk/super/0.disk", ios::in);
    setting >> formatted;
    setting >> idle_block_list_index;
    setting.close();
}

void writeSetting() {
    setting.open("./disk/super/0.disk", ios::out);
    setting << formatted << endl;
    setting << idle_block_list_index << endl;
    setting.close();
}

// Block 1#
void readUser() {
    user.open("./disk/super/1.disk", ios::in);
    users.clear();
    string name, pwd;
    int uid, gid, role;
    while (user >> name >> pwd >> uid >> gid >> role) {
        users[name] = User(pwd, uid, gid, role);
        name_of_user[uid] = name;
        name_of_group[gid] = name;  // TODO: add group
    }
    user.close();
}

void writeUser() {
    user.open("./disk/super/1.disk", ios::out);
    map<string, User>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        User u = it->second;
        user << it->first << " " << u.password << " " << u.id << " " << u.gid << " " << u.role << endl;
    }
    user.close();
}

// Block 2#
void readIdleInode() {
    block.open("./disk/super/2.disk", ios::in);
    idle_inode_stack.clear();
    int x;
    while (block >> x) {
        idle_inode_stack.push_back(x);
    }
    block.close();
}

void writeIdleInode() {
    block.open("./disk/super/2.disk", ios::out);
    for (unsigned i = 0; i < idle_inode_stack.size(); i++) {
        if (i) block << ' ';
        block << idle_inode_stack[i];
    }
    block << endl;
    block.close();
}

// Block 3# ~ 9#
string getIdleBlockPath(int index) {
    int blockNum = index + 3;
    stringstream ss;
    ss << blockNum;
    string blockNumStr;
    ss >> blockNumStr;
    string blockPath = "./disk/super/" + blockNumStr + ".disk";
    return blockPath;
}

void readIdleBlock(int index) {
    block.open(getIdleBlockPath(index).c_str(), ios::in);
    idle_block_stack.clear();
    int x;
    while (block >> x) {
        idle_block_stack.push_back(x);
    }
    block.close();
}

void writeIdleBlock(int index) {
    block.open(getIdleBlockPath(index).c_str(), ios::out);
    for (unsigned i = 0; i < idle_block_stack.size(); i++) {
        if (i) block << ' ';
        block << idle_block_stack[i];
    }
    block << endl;
    block.close();
}

void popIdleBlockList() {
    if (idle_block_list_index) {
        writeIdleBlock(idle_block_list_index - 1);
    }
    readIdleBlock(idle_block_list_index);
    idle_block_list_index++;
}

void pushIdleBlockList() {
    idle_block_list_index--;
    writeIdleBlock(idle_block_list_index);
}

// Block 10# ~ 19#
string getInodePath(int index) {
    int blockNum = index / 64 + 10;
    stringstream ss;
    ss << blockNum;
    string blockNumStr;
    ss >> blockNumStr;
    string blockPath = "./disk/inode/" + blockNumStr + ".disk";
    return blockPath;
}

void readInodeOneBlock(int index) {
    block.open(getInodePath(index).c_str(), ios::in);

    int offset = index / 64 * 64;
    for (int i = offset; i < offset+64; i++) {
        Inode inode;
        block >> inode.mode;
        block >> inode.link_cnt;
        block >> inode.uid;
        block >> inode.gid;
        block >> inode.file_size;
        block >> inode.block_cnt;
        for (int j = 0; j < 10; j++) {
            block >> inode.addr[j];
        }
        block >> inode.addr1;
        inodes[i] = inode;
    }

    block.close();
}

void writeInodeOneBlock(Inode inode, int index) {
    block.open(getInodePath(index).c_str(), ios::out);

    inodes[index] = inode;

    int offset = index / 64 * 64;
    for (int i = offset; i < offset+64; i++) {
        Inode node = inodes[i];
        block << node.mode << endl;
        block << node.link_cnt << endl;
        block << node.uid << endl;
        block << node.gid << endl;
        block << node.file_size << endl;
        block << node.block_cnt << endl;
        if (node.mode[0] == 'd') {
            node.file_size = node.block_cnt * 4096;
        }
        for (int j = 0; j < 10; j++) {
            if (j) block << " ";
            block << node.addr[j];
        }
        block << endl;
        block << node.addr1 << endl;
        //block << endl;
    }

    block.close();
}

void writeInodeAll() {
    int cnt = 0;
    map<int, Inode>::iterator it;
    block.open(getInodePath(0).c_str(), ios::out);
    for (it = inodes.begin(); it != inodes.end(); it++) {
        Inode inode = it->second;
        block << inode.mode << endl;
        block << inode.link_cnt << endl;
        block << inode.uid << endl;
        block << inode.gid << endl;
        block << inode.file_size << endl;
        block << inode.block_cnt << endl;
        for (int i = 0; i < 10; i++) {
            if (i) block << " ";
            block << inode.addr[i];
        }
        block << endl;
        block << inode.addr1 << endl;
        //block << endl;

        if (++cnt % 64 == 0 && cnt / 64 != 90) {
            // switch block
            block.close();
            block.open(getInodePath(cnt).c_str(), ios::out);
        }
    }
    block.close();
}

Inode getInode(int index) {
    if (inodes.find(index) == inodes.end()) {
        readInodeOneBlock(index);
    }
    return inodes[index];
}

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
            if (inode.addr1 == -1) break;
            vector<int> indirect_blocks;
            readIndirectBlock(inode.addr1, indirect_blocks);
            blocks.insert(blocks.end(), indirect_blocks.begin(), indirect_blocks.end());
            blocks.push_back(inode.addr1);
            break;
        }
    }
    releaseBlock(blocks);
    releaseInode(inode.index);
    writeInodeOneBlock(Inode(), inode.index);
}

void rebuildInode(Inode &inode, int block_cnt) {
    //cout << inode.block_cnt << " " << block_cnt << " ";  // debug
    if (block_cnt == inode.block_cnt) return;
    if (block_cnt > inode.block_cnt) {
        unsigned num = block_cnt - inode.block_cnt;
        //cout << num << endl;  // debug
        vector<int> blocks = allocateBlock(num);
        if (blocks.size() != num) return;
        for (unsigned i = inode.block_cnt, j = 0; i < (unsigned)block_cnt && j < blocks.size(); i++, j++) {
            if (i < 10) {
                inode.addr[i] = blocks[j];
            } else {
                // Indirect
                vector<int> indirect_blocks;
                if (inode.addr1 == -1) {
                    inode.addr1 = allocateBlock();
                } else {
                    readIndirectBlock(inode.addr1, indirect_blocks);
                }
                indirect_blocks.insert(indirect_blocks.end(), blocks.begin()+j, blocks.end());
                writeIndirectBlock(inode.addr1, indirect_blocks);
                break;
            }
        }
    } else if (block_cnt < inode.block_cnt) {
        vector<int> blocks;
        for (int i = block_cnt; i < inode.block_cnt; i++) {
            if (i < 10) {
                blocks.push_back(inode.addr[i]);
                inode.addr[i] = -1;
            } else {
                // Indirect
                vector<int> indirect_blocks;
                readIndirectBlock(inode.addr1, indirect_blocks);
                blocks.insert(blocks.end(), indirect_blocks.begin()+i-10, indirect_blocks.end());
                indirect_blocks.erase(indirect_blocks.begin()+i-10, indirect_blocks.end());
                if (indirect_blocks.empty()) {
                    blocks.push_back(inode.addr1);
                    inode.addr1 = -1;
                }
                writeIndirectBlock(inode.addr1, indirect_blocks);
                break;
            }
        }
        releaseBlock(blocks);
    }
    inode.block_cnt = block_cnt;
    writeInodeOneBlock(inode, inode.index);
}

// Block 100# ~ 1023#
string getDataPath(int index) {
    int blockNum = index + 100;
    stringstream ss;
    ss << blockNum;
    string blockNumStr;
    ss >> blockNumStr;
    string blockPath = "./disk/data/" + blockNumStr + ".disk";
    return blockPath;
}

void readIndirectBlock(int block_index, vector<int> &blocks) {
    block.open(getDataPath(block_index).c_str(), ios::in);

    blocks.clear();
    int index;
    while (block >> index) {
        blocks.push_back(index);
    }

    block.close();
}

void writeIndirectBlock(int block_index, vector<int> &blocks) {
    block.open(getDataPath(block_index).c_str(), ios::out);

    for (unsigned i = 0; i < blocks.size(); i++) {
        block << blocks[i] << endl;
    }

    block.close();
}

void readDirectory(int index, int block_index) {
    block.open(getDataPath(block_index).c_str(), ios::in);

    string name;
    int inode;
    while (block >> name >> inode) {
        directories[index][name] = inode;
    }

    block.close();
}

void readDirectory(int index) {
    Inode inode = getInode(index);

    if (inode.mode[0] != 'd') {
        return;  // not directory
    }

    directories[index].clear();

    for (int i = 0; i < inode.block_cnt; i++) {
        if (i < 10) {
            readDirectory(index, inode.addr[i]);
        } else {
            // Indirect
            vector<int> indirect_blocks;
            readIndirectBlock(inode.addr1, indirect_blocks);
            for (unsigned j = 0; j < indirect_blocks.size(); j++) {
                readDirectory(index, indirect_blocks[j]);
            }
            break;
        }
    }
}

void writeDirectory(int index, int block_index, map<string, int>::iterator &it) {
    block.open(getDataPath(block_index).c_str(), ios::out);

    int cnt = 0;
    for ( ; cnt < 10 && it != directories[index].end(); it++, cnt++) {
        block << it->first << " " << it->second << endl;
    }

    block.close();
}

void writeDirectory(int index) {
    Inode inode = getInode(index);

    if (inode.mode[0] != 'd') {
        return;  // not directory
    }

    rebuildInode(inode, (directories[index].size()+9) / 10);

    map<string, int>::iterator it = directories[index].begin();

    for (int i = 0; i < inode.block_cnt; i++) {
        if (i < 10) {
            writeDirectory(index, inode.addr[i], it);
        } else {
            // Indirect
            vector<int> indirect_blocks;
            readIndirectBlock(inode.addr1, indirect_blocks);
            for (unsigned j = 0; j < indirect_blocks.size(); j++) {
                writeDirectory(index, indirect_blocks[j], it);
            }
            break;
        }
    }
}

map<string, int> getDirectory(int index) {
    if (directories.find(index) == directories.end()) {
        readDirectory(index);
    }
    return directories[index];
}

void readData(int index, int block_index, string &data) {
    block.open(getDataPath(block_index).c_str(), ios::in);

    string line;
    data.clear();
    int cnt = 0;
    while (getline(block, line)) {
        if (cnt++) data += '\n';
        data += line;
    }

    block.close();
}

void readData(int index, string &data) {
    Inode inode = getInode(index);

    if (inode.mode[0] != '-') {
        return;  // not file
    }

    string part_data;
    for (int i = 0; i < inode.block_cnt; i++) {
        if (i < 10) {
            readData(index, inode.addr[i], part_data);
            data += part_data;
        } else {
            // Indirect
            vector<int> indirect_blocks;
            readIndirectBlock(inode.addr1, indirect_blocks);
            for (unsigned j = 0; j < indirect_blocks.size(); j++) {
                readData(index, indirect_blocks[j], part_data);
                data += part_data;
            }
            break;
        }
    }
}

void writeData(int index, int block_index, string &data, string::iterator &it) {
    block.open(getDataPath(block_index).c_str(), ios::out);

    int cnt = 0;
    for ( ; cnt < 20 && it != data.end(); it++, cnt++) {
        block << *it;
    }

    block.close();
}

void writeData(int index, string &data) {
    Inode inode = getInode(index);

    if (inode.mode[0] != '-') {
        return;  // not file
    }

    rebuildInode(inode, (data.size()+19) / 20);

    string::iterator it = data.begin();

    for (int i = 0; i < inode.block_cnt; i++) {
        if (i < 10) {
            writeData(index, inode.addr[i], data, it);
        } else {
            // Indirect
            vector<int> indirect_blocks;
            readIndirectBlock(inode.addr1, indirect_blocks);
            for (unsigned j = 0; j < indirect_blocks.size(); j++) {
                writeData(index, indirect_blocks[j], data, it);
            }
            break;
        }
    }

    writeInodeOneBlock(inode, index);
}

#endif // _BASE_H
