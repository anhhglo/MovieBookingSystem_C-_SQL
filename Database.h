#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>
#include <string>
#include <iostream>

class Database {
private:
    MYSQL* conn;
public:
    Database();
    ~Database();
    bool connect(const char* host, const char* user, const char* pass, const char* db, unsigned int port=3306);
    bool execute(const std::string& query);
    MYSQL_RES* queryResult(const std::string& query);
    MYSQL* getConnection();
};

#endif
