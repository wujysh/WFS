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

}
