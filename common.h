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

string cur_dir;
vector<int> path;  // whole path from root to current directory
bool formatted;
string username;
map<string, string> users;

// Global structures
struct FileSystem {
    stack<int> idle_inode_stack;
    stack<int> idle_block_stack;
    FileSystem() {}
    FileSystem(stack<int> _iis, stack<int> _ibs)
    : idle_inode_stack(_iis), idle_block_stack(_ibs) {}
} filesystem;

struct Inode {
    string mode;  //[0]类型(d目录-文件)[1~3]owner权限[4~6]group权限[7~9]other权限 r(读)w(写)x(执行)
    string uid;  // user id
    string gid;  // group id
    int file_size;
    int block_cnt;
    vector<int> addr;  // direct
    int addr1;  // single indirect
    Inode() : addr(10, -1) {}
    Inode(string _mode, string _uid, string _gid, int _fs, int _bc, vector<int> _addr, int _addr1)
    : mode(_mode), uid(_uid), gid(_gid), file_size(_fs), block_cnt(_bc), addr(_addr), addr1(_addr1) {}
};

struct Directory {
    string name;
    int inode;
    Directory() {}
    Directory(string _name, int _inode) : name(_name), inode(_inode) {}
};


// Global functions

