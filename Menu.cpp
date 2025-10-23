#include "Menu.h"
#include "Customer.h"
#include "Movie.h"
#include "Cinema.h"
#include "Booking.h"
#include <iostream>
#include <sstream>
using namespace std;


// 🟩 Hàm đăng nhập admin
bool Menu::adminLogin(Database& db) {
    string user, pass;
    cout << "\n=== Admin Login ===\n";
    cout << "Username: "; cin >> user;
    cout << "Password: "; cin >> pass;

    MYSQL* conn = db.getConnection();  // ✅ đúng cách
    if (!conn) return false;

    string query = "SELECT * FROM admin_tb WHERE username='" + user + "' AND password='" + pass + "'";
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        bool ok = (mysql_num_rows(res) > 0);
        mysql_free_result(res);
        if (ok) {
            cout << "✅ Login successful!\n";
            return true;
        } else {
            cout << "❌ Invalid admin credentials.\n";
        }
    } else {
        cerr << "Query failed: " << mysql_error(conn) << endl;
    }
    return false;
}

// 🟩 Menu dành cho admin
void Menu::showAdminMenu(Database& db) {
    int choice;
    while (true) {
        cout << "\n=== Admin Menu ===\n";
        cout << "1. Manage Movies\n";
        cout << "2. Manage Cinemas\n";
        cout << "3. Logout\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1)
            Movie::manageMovies(db);
        else if (choice == 2)
            Cinema::manageCinemas(db);
        else if (choice == 3)
            break;
        else
            cout << "Invalid choice.\n";
    }
}

// 🟩 Hàm đăng nhập khách hàng
bool Menu::customerLogin(Database& db, int& customer_id, string& customer_name) {
    string user, pass;
    cout << "\n=== Customer Login ===\n";
    cout << "Username: "; cin >> user;
    cout << "Password: "; cin >> pass;

    string query = "SELECT customer_id, name FROM customer_tb WHERE username='" + user + "' AND password='" + pass + "'";
    MYSQL_RES* res = db.queryResult(query);
    if (!res) return false;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row) {
        customer_id = stoi(row[0]);
        customer_name = row[1];
        mysql_free_result(res);
        cout << "✅ Welcome, " << customer_name << "!\n";
        return true;
    } else {
        cout << "❌ Wrong username or password.\n";
        mysql_free_result(res);
        return false;
    }
}

// 🟩 Đăng ký khách hàng mới
void Menu::registerCustomer(Database& db) {
    string name, phone, email, user, pass;
    cin.ignore();
    cout << "\n=== Register New Account ===\n";
    cout << "Full name: "; getline(cin, name);
    cout << "Phone: "; getline(cin, phone);
    cout << "Email: "; getline(cin, email);
    cout << "Username: "; getline(cin, user);
    cout << "Password: "; getline(cin, pass);

    string query = "INSERT INTO customer_tb (name, phone, email, username, password) VALUES ('" +
        name + "', '" + phone + "', '" + email + "', '" + user + "', '" + pass + "')";
    if (db.execute(query))
        cout << "✅ Registration successful! You can now log in.\n";
    else
        cout << "❌ Failed to register. Maybe username exists.\n";
}

// 🟩 Menu dành cho khách hàng
void Menu::showCustomerMenu(Database& db, int customer_id, string customer_name) {
    int choice;
    while (true) {
        cout << "\n=== Customer Menu ===\n";
        cout << "1. List Movies\n";
        cout << "2. Book Ticket\n";
        cout << "3. Logout\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1)
            Movie::listMovies(db);
        else if (choice == 2) {
            int screening_id, seat_count;
            string seats;
            double total_price;
            cout << "Enter screening_id: "; cin >> screening_id;
            cout << "Seats (e.g. A1,A2): "; cin >> ws; getline(cin, seats);
            cout << "Seat count: "; cin >> seat_count;

            MYSQL_RES* r = db.queryResult("SELECT ticket_price FROM screening_tb WHERE screening_id=" + to_string(screening_id));
            if (!r) continue;
            MYSQL_ROW rr = mysql_fetch_row(r);
            if (!rr) { cout << "Screening not found.\n"; mysql_free_result(r); continue; }
            total_price = atof(rr[0]) * seat_count;
            mysql_free_result(r);

            Booking b(customer_id, screening_id, seats, seat_count, total_price);
            if (Booking::createBooking(db, b))
                cout << "✅ Booking successful! QR: " << b.qr << endl;
            else
                cout << "❌ Booking failed.\n";
        } else if (choice == 3) break;
        else cout << "Invalid choice.\n";
    }
}
// 🟩 Hàm hiển thị menu chính
void Menu::show(Database& db) {
    int choice;
    while (true) {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Admin Login\n";
        cout << "2. Customer Login\n";
        cout << "3. Register New Customer\n";
        cout << "4. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
            if (adminLogin(db))
                showAdminMenu(db);
        }
        else if (choice == 2) {
            int customer_id;
            string customer_name;
            if (customerLogin(db, customer_id, customer_name))
                showCustomerMenu(db, customer_id, customer_name);
        }
        else if (choice == 3) {
            registerCustomer(db);
        }
        else if (choice == 4) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }
}

