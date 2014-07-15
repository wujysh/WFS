#ifndef _COMMON_H
#define _COMMON_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
#include <ctime>
#include <stack>
#include <list>
#include <set>
#include <map>
using namespace std;

// Global variables
fstream block, user, setting;

bool formatted;
string cur_dir;

// Global structures
struct Path {
    int inode;
    string name;
    Path() {}
    Path(int _inode, string _name) : inode(_inode), name(_name) {}
};
vector<Path> path;  // whole path from root to current directory

struct User {
    string password;
    int id;
    int gid;
    int role; // 0-normal user, 1-administrator
    map<int, int> openFiles;
    User() {}
    User(string _pwd, int _id, int _gid, int _role) : password(_pwd), id(_id), gid(_gid), role(_role) {}
};
map<string, User> users;
map<int, string> name_of_user, name_of_group;
string username;

vector<int> idle_inode_stack;
vector<int> idle_block_stack;

struct Inode {
    // only memory Inode
    int flag;
    int open_cnt;
    int index;
    // both memory and disk Inode
    string mode;  // type (directory or file) and authority flag like "drwxrwxrwx"
    int link_cnt;  // number of links
    int uid;  // user id
    int gid;  // group id
    int file_size;
    int block_cnt;
    vector<int> addr;  // direct
    int addr1;  // single indirect
    //time_t access_time;
    //time_t modify_time;
    //time_t create_time;
    Inode() : flag(0), open_cnt(0), index(-1), mode("drwxr-x--x"), link_cnt(0), uid(-1), gid(-1), file_size(0),
        block_cnt(0), addr(10, -1), addr1(-1) {}
    Inode(int _index, string _mode, int _uid, int _gid, int _fs, int _bc, vector<int> _addr, int _addr1/*, time_t _at, time_t _mt, time_t _ct*/) :
        flag(0), open_cnt(0), index(_index), mode(_mode), link_cnt(1), uid(_uid), gid(_gid), file_size(_fs),
        block_cnt(_bc), addr(_addr), addr1(_addr1)/*, access_time(_at), modify_time(_mt), create_time(_ct)*/ {}
    Inode(int _index, string _mode, int _uid, int _gid, int _fs, int _bc/*, vector<int> _addr, int _addr1, time_t _at, time_t _mt, time_t _ct*/) :
        flag(0), open_cnt(0), index(_index), mode(_mode), link_cnt(1), uid(_uid), gid(_gid), file_size(_fs),
        block_cnt(_bc), addr(10, -1), addr1(-1)/*, access_time(_at), modify_time(_mt), create_time(_ct)*/ {}
};
map<int, Inode> inodes;

map<int, map<string, int> > directories;

struct OpenFile {
    int flag;
    int count;
    int index;
    int offset;
    OpenFile() {}
    OpenFile(int _flag, int _count, int _index, int _offset) : flag(_flag), count(_count), index(_index), offset(_offset) {}
};
vector<OpenFile> openFiles;

// Global functions
void clearScreen() {
#ifdef __linux__
    system("clear");
#else
    system("cls");
#endif
}

#endif // _COMMON_H
