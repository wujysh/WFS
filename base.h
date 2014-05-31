void readSetting(bool &formatted) {
    setting.open("setting.ini", ios::in);
    setting >> formatted;
    setting.close();
}

void writeSetting(bool &formatted) {
    setting.open("setting.ini", ios::out);
    setting << formatted << endl;
    setting.close();
}

void readUser() {
    user.open("user.ini", ios::in);
    users.clear();
    string name, pwd;
    while (user >> name >> pwd) {
        users[name] = pwd;
    }
    user.close();
}

void writeUser() {
    user.open("user.ini", ios::out);
    map<string, string>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        user << it->first << " " << it->second << endl;
    }
    user.close();
}
