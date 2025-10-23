#include "Database.h"
#include "Menu.h"
#include <iostream>

int main() {
    Database db;
    if (!db.connect("127.0.0.1", "root", "anh1152004.", "movieticketdb", 3306)) {
        std::cerr << "Cannot connect DB. Check credentials.\n";
        return 1;
    }
    Menu menu;
    menu.show(db);
    return 0;
}
