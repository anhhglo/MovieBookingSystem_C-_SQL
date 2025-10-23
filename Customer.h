#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "Database.h"

class Customer {
public:
    static bool login(Database& db, int& customer_id, std::string& customer_name);
    static void registerAccount(Database& db);
};

#endif
