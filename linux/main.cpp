#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <unistd.h>

using namespace std;

string current_time() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

vector<pair<string, string>> load_users() {
    vector<pair<string, string>> users;
    ifstream file("users.txt");
    if (!file.is_open()) {
        ofstream newfile("users.txt");
        if (newfile.is_open()) {
            newfile << "admin:admin123\n";
            newfile.close();
            users.push_back({"admin", "admin123"});
        }
        return users;
    }
    string line;
    while (getline(file, line)) {
        size_t colon = line.find(':');
        if (colon != string::npos) {
            string user = line.substr(0, colon);
            string pass = line.substr(colon + 1);
            users.push_back({user, pass});
        }
    }
    file.close();
    return users;
}

bool save_users(const vector<pair<string, string>>& users) {
    ofstream file("users.txt");
    if (!file.is_open()) return false;
    for (const auto& u : users) {
        file << u.first << ":" << u.second << "\n";
    }
    file.close();
    return true;
}

bool authenticate(const string& username, const string& password, const vector<pair<string, string>>& users) {
    for (const auto& u : users) {
        if (u.first == username && u.second == password) return true;
    }
    return false;
}

bool add_user(const string& new_username, const string& new_password, vector<pair<string, string>>& users) {
    for (const auto& u : users) {
        if (u.first == new_username) return false;
    }
    users.push_back({new_username, new_password});
    return save_users(users);
}

bool delete_user(const string& username, vector<pair<string, string>>& users) {
    auto it = remove_if(users.begin(), users.end(), [&](const pair<string, string>& u) {
        return u.first == username;
    });
    if (it != users.end()) {
        users.erase(it, users.end());
        return save_users(users);
    }
    return false;
}

void list_users(const vector<pair<string, string>>& users) {
    cout << "\n--- User list ---\n";
    for (const auto& u : users) {
        cout << u.first << " : " << u.second << "\n";
    }
    cout << "----------------\n";
}

void write_log(const string& username) {
    string log_line = "[" + current_time() + "] logger: user " + username + " logged in\n";
    cout << log_line;
    ofstream logfile("logs.txt", ios::app);
    if (logfile.is_open()) {
        logfile << log_line;
        logfile.close();
    } else {
        cerr << "Warning: cannot open logs.txt\n";
    }
}

void admin_menu(vector<pair<string, string>>& users) {
    int choice;
    do {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add user\n";
        cout << "2. Delete user\n";
        cout << "3. List users\n";
        cout << "4. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                string new_user, new_pass;
                cout << "New username: ";
                getline(cin, new_user);
                new_pass = getpass("New password: ");
                if (add_user(new_user, new_pass, users)) {
                    cout << "User " << new_user << " added successfully.\n";
                } else {
                    cout << "User already exists or error.\n";
                }
                break;
            }
            case 2: {
                string del_user;
                cout << "Username to delete: ";
                getline(cin, del_user);
                if (del_user == "admin") {
                    cout << "Cannot delete admin.\n";
                } else if (delete_user(del_user, users)) {
                    cout << "User " << del_user << " deleted.\n";
                } else {
                    cout << "User not found.\n";
                }
                break;
            }
            case 3:
                list_users(users);
                break;
            case 4:
                cout << "Exiting admin menu.\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

int main() {
    vector<pair<string, string>> users = load_users();

    string username, password;

    cout << "Username: ";
    getline(cin, username);
    password = getpass("Password: ");

    if (!authenticate(username, password, users)) {
        cout << "Invalid username or password.\n";
        return 1;
    }

    write_log(username);

    if (username == "admin") {
        admin_menu(users);
    }

    return 0;
}