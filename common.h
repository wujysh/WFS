#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <set>
#include <map>
using namespace std;

// Global variables
fstream block, user, setting;

bool formatted;
string cur_dir;

struct Path {
    int inode;
    string name;
};
vector<Path> path;  // whole path from root to current directory

struct User {
    string password;
    int id;
    int gid;
    int role; // 0-normal user, 1-administrator
    User() {}
    User(string _pwd, int _id, int _gid, int _role) : password(_pwd), id(_id), gid(_gid), role(_role) {}
};
map<string, User> users;
string username;

vector<int> idle_inode_stack;
vector<int> idle_block_stack;

// Global structures
struct Inode {
    string mode;  //[0]类型(d目录-文件)[1~3]owner权限[4~6]group权限[7~9]other权限 r(读)w(写)x(执行)
    int uid;  // user id
    int gid;  // group id
    int file_size;
    int block_cnt;
    vector<int> addr;  // direct
    int addr1;  // single indirect
    Inode() : mode("----------"), uid(-1), gid(-1), file_size(0), block_cnt(0), addr(10, -1), addr1(-1) {}
    Inode(string _mode, int _uid, int _gid, int _fs, int _bc, vector<int> _addr, int _addr1)
    : mode(_mode), uid(_uid), gid(_gid), file_size(_fs), block_cnt(_bc), addr(_addr), addr1(_addr1) {}
};
map<int, Inode> inodes;

struct Directory {
    string name;
    int inode;
    Directory() {}
    Directory(string _name, int _inode) : name(_name), inode(_inode) {}
};
map<int, vector<Directory> > directories;  // Block No. => Directory structure


// Global functions

