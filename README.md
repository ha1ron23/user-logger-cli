# User Logger CLI

A simple command-line tool for user authentication and logging.  
When a user logs in successfully, the event is recorded in `logs.txt` with a timestamp.  
Admin users can add, delete, and list users via an interactive menu.

## Features

- **Login with username and password** (password hidden while typing)
- **Logs every successful login** to `logs.txt` with timestamp
- **Admin panel** (only for user `admin`):
  - Add new users
  - Delete users (except `admin`)
  - List all registered users
- **Persistent storage** – users saved in `users.txt` (format: `username:password`)
- **No external dependencies** – uses only C++ standard library and POSIX `getpass`

## Install

```bash
git clone https://github.com/ha1ron23/user-logger-cli.git
cd user-logger-cli
cd linux # or cd win or cd mac
```

## Compile and run

```bash
g++ -std=c++11 main.cpp -o user-logger 
./user-logger
```

## Default admin credentials
After first run, a default admin account is created:

  Username: admin
  Password: admin123

Change the password by editing users.txt or via admin menu

## Example
```bash
$ ./user-logger
Username: ha1ron23
Password: 
[2025-05-19 14:32:11] logger: user ha1ron23 logged in 

$ ./user-logger
Username: admin
Password: 
[2025-05-19 14:35:22] logger: user admin logged in 

--- Admin Menu ---
1. Add user
2. Delete user
3. List users
4. Exit
Choice: 3
--- User list ---
admin : admin123
ha1ron23 : starthisrepo
----------------
```

## License
MIT License

## Contributing
Feel free to open issues or PRs for improvements like:

  Password hashing (SHA256)
  Configurable log file path
