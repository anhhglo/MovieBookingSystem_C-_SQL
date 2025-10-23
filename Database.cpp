#include "Database.h"

Database::Database() {
    conn = mysql_init(nullptr);
}

Database::~Database() {
    if (conn) mysql_close(conn);
}

bool Database::connect(const char* host, const char* user, const char* pass, const char* db, unsigned int port) {
    unsigned int ssl_mode = SSL_MODE_DISABLED;
    mysql_options(conn, MYSQL_OPT_SSL_MODE, &ssl_mode);

    conn = mysql_real_connect(conn, host, user, pass, db, port, NULL, 0);
    if (!conn) {
        std::cerr << "DB connect failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    std::cout << "DB connected.\n";
    return true;
}

bool Database::execute(const std::string& query) {
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "Query failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

MYSQL_RES* Database::queryResult(const std::string& query) {
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "Query failed: " << mysql_error(conn) << std::endl;
        return nullptr;
    }
    return mysql_store_result(conn);
}

MYSQL* Database::getConnection() {
    return conn;
}
