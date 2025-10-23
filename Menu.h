#ifndef MENU_H
#define MENU_H

#include "Database.h"
#include <string>
using namespace std;

class Menu {
public:
    void show(Database& db);

    // 🟩 Thêm các hàm đã triển khai trong Menu.cpp
    static bool adminLogin(Database& db);
    static void showAdminMenu(Database& db);

    static bool customerLogin(Database& db, int& customer_id, string& customer_name);
    static void registerCustomer(Database& db);
    static void showCustomerMenu(Database& db, int customer_id, string customer_name);
};

#endif
