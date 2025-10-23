#include "Cinema.h"
#include <sstream>
#include <iostream>

Cinema::Cinema() {}
Cinema::Cinema(int id, const std::string& name, const std::string& address)
: id(id), name(name), address(address) {}

bool Cinema::addCinema(Database& db, const Cinema& c) {
    std::ostringstream q;
    q << "INSERT INTO cinema_tb (name, address) VALUES ('" << c.name << "', '" << c.address << "')";
    return db.execute(q.str());
}

void Cinema::listCinemas(Database& db) {
    MYSQL_RES* res = db.queryResult("SELECT cinema_id, name, address FROM cinema_tb");
    if (!res) return;
    MYSQL_ROW row;
    printf("-------------------------------\n");
    printf("| %-5s | %-15s | %-20s |\n", "ID", "Name", "Address");
    while ((row = mysql_fetch_row(res))) {
        printf("| %-5s | %-15s | %-20s |\n", row[0], row[1], row[2]);
    }
    printf("-------------------------------\n");
    mysql_free_result(res);
}

void Cinema::manageCinemas(Database& db) {
    int choice;
    while (true) {
        std::cout << "\n=== Manage Cinemas ===\n";
        std::cout << "1. List Cinemas\n2. Add Cinema\n3. Back\nChoose: ";
        std::cin >> choice;
        if (choice == 1) {
            listCinemas(db);
        } else if (choice == 2) {
            std::string name, addr;
            std::cin.ignore();
            std::cout << "Cinema name: "; getline(std::cin, name);
            std::cout << "Address: "; getline(std::cin, addr);
            Cinema c(0, name, addr);
            if (addCinema(db, c))
                std::cout << "Cinema added.\n";
            else
                std::cout << "Failed to add cinema.\n";
        } else if (choice == 3) break;
        else std::cout << "Invalid choice.\n";
    }
}

