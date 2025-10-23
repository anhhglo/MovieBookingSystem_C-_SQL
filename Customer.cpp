#include "Customer.h"
#include <iostream>
#include <sstream>
#include <mysql.h>

using namespace std;

bool Customer::login(Database& db, int& customer_id, string& customer_name) {
    string username, password;
    cout << "\n--- CUSTOMER LOGIN ---\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    ostringstream query;
    query << "SELECT customer_id, name FROM customer_tb "
          << "WHERE username='" << username << "' AND password='" << password << "'";

    MYSQL_RES* res = db.queryResult(query.str());
    if (res && mysql_num_rows(res) > 0) {
        MYSQL_ROW row = mysql_fetch_row(res);
        customer_id = stoi(row[0]);
        customer_name = row[1];
        mysql_free_result(res);
        cout << "Login successful! Welcome, " << customer_name << "!\n";
        return true;
    } else {
        cout << "Invalid username or password.\n";
        return false;
    }
}

void Customer::registerAccount(Database& db) {
    string name, phone, email, username, password;

    cout << "\n--- REGISTER NEW CUSTOMER ---\n";
    cout << "Full name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Phone: ";
    cin >> phone;
    cout << "Email: ";
    cin >> email;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    ostringstream query;
    query << "INSERT INTO customer_tb (name, phone, email, username, password) VALUES ('"
          << name << "', '" << phone << "', '" << email << "', '" << username << "', '" << password << "')";

    if (db.execute(query.str()))
        cout << "Registration successful! You can now log in.\n";
    else
        cout << "Error: Could not register account.\n";
}
