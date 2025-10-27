#include "Hall.h"
#include <iostream>
#include <sstream>
#include <mysql.h>
using namespace std;

Hall::Hall() {}
Hall::Hall(int id, int cid, const string& name, int seats)
    : hall_id(id), cinema_id(cid), hall_name(name), seat_count(seats) {}

bool Hall::addHall(Database& db, const Hall& h) {
    ostringstream q;
    q << "INSERT INTO hall_tb (cinema_id, hall_name, seat_count) VALUES ("
      << h.cinema_id << ", '" << h.hall_name << "', " << h.seat_count << ")";
    return db.execute(q.str());
}

void Hall::listHalls(Database& db) {
    MYSQL_RES* res = db.queryResult(
        "SELECT h.hall_id, c.name, h.hall_name, h.seat_count "
        "FROM hall_tb h JOIN cinema_tb c ON h.cinema_id = c.cinema_id");
    if (!res) return;

    MYSQL_ROW row;
    printf("--------------------------------------------------------------\n");
    printf("| %-5s | %-15s | %-10s | %-10s |\n", "ID", "Cinema", "Hall", "Seats");
    while ((row = mysql_fetch_row(res))) {
        printf("| %-5s | %-15s | %-10s | %-10s |\n",
               row[0], row[1], row[2], row[3]);
    }
    printf("--------------------------------------------------------------\n");
    mysql_free_result(res);
}

void Hall::manageHalls(Database& db) {
    int choice;
    while (true) {
        cout << "\n=== Manage Halls ===\n";
        cout << "1. List Halls\n";
        cout << "2. Add Hall\n";
        cout << "3. Back\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
            listHalls(db);
        } else if (choice == 2) {
            int cid, seats;
            string name;
            cout << "Cinema ID: ";
            cin >> cid;
            cin.ignore();
            cout << "Hall name: ";
            getline(cin, name);
            cout << "Seat count: ";
            cin >> seats;

            Hall h(0, cid, name, seats);
            if (addHall(db, h))
                cout << "✅ Hall added successfully!\n";
            else
                cout << "❌ Failed to add hall.\n";
        } else if (choice == 3)
            break;
        else
            cout << "Invalid choice.\n";
    }
}
