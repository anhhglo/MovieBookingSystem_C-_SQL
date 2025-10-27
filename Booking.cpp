#include "Booking.h"
#include "Database.h"
#include <iostream>
#include <sstream>
#include <set>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <ctime>
#include "qrcodegen.hpp"

using namespace std;
using namespace qrcodegen;

// 🟩 In mã QR thật ra console
void Booking::printQRCodeToConsole(const string &text) {
    const QrCode qr = QrCode::encodeText(text.c_str(), QrCode::Ecc::LOW);
    int border = 2;
    for (int y = -border; y < qr.getSize() + border; y++) {
        for (int x = -border; x < qr.getSize() + border; x++) {
            cout << (qr.getModule(x, y) ? "██" : "  ");
        }
        cout << "\n";
    }
}

// 🟩 Sinh dữ liệu QR (thật)
string Booking::generateQRCodeData(int booking_id) {
    ostringstream data;
    data << "BookingID:" << booking_id
         << "|System:MovieTicket"
         << "|Time:" << time(nullptr);
    return data.str();
}

// 🟩 Hiển thị sơ đồ ghế
void Booking::showSeatMatrix(Database& db, int screening_id) {
    ostringstream q1;
    q1 << "SELECT h.seat_count "
        << "FROM screening_tb s "
        << "JOIN hall_tb h ON s.hall_id = h.hall_id "
        << "WHERE s.screening_id = " << screening_id;

    MYSQL_RES* res = db.queryResult(q1.str());
    if (!res) {
        cout << "Cannot query seat info.\n";
        return;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        cout << "Screening not found.\n";
        mysql_free_result(res);
        return;
    }

    int totalSeats = atoi(row[0]);
    mysql_free_result(res);

    ostringstream q2;
    q2 << "SELECT seats FROM booking_tb WHERE screening_id = " << screening_id;
    MYSQL_RES* res2 = db.queryResult(q2.str());

    set<string> bookedSeats;
    MYSQL_ROW row2;
    while ((row2 = mysql_fetch_row(res2))) {
        string seatStr = row2[0];
        istringstream ss(seatStr);
        string token;
        while (getline(ss, token, ',')) {
            bookedSeats.insert(token);
        }
    }
    mysql_free_result(res2);

    int n = sqrt(totalSeats);
    if (n * n < totalSeats) n++;

    cout << "\n--- SEAT MAP --- (X = booked)\n";
    for (int i = 0; i < n; ++i) {
        char rowLabel = 'A' + i;
        for (int j = 1; j <= n; ++j) {
            int seatIndex = i * n + j;
            if (seatIndex > totalSeats) break;
            ostringstream seat;
            seat << rowLabel << j;

            if (bookedSeats.count(seat.str()))
                cout << setw(4) << "X";
            else
                cout << setw(4) << seat.str();
        }
        cout << "\n";
    }
    cout << "-----------------------------\n";
}

// 🟩 Tạo đơn đặt vé
bool Booking::createBooking(Database& db, Booking& b) {
    ostringstream qcheck;
    qcheck << "SELECT seats FROM booking_tb WHERE screening_id = " << b.screening_id;
    MYSQL_RES* res = db.queryResult(qcheck.str());
    if (!res) return false;
    set<string> booked;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        string seatStr = row[0];
        istringstream ss(seatStr);
        string token;
        while (getline(ss, token, ',')) booked.insert(token);
    }
    mysql_free_result(res);

    istringstream userSeats(b.seats);
    string seat;
    while (getline(userSeats, seat, ',')) {
        if (booked.count(seat)) {
            cout << "❌ Seat " << seat << " is already booked!\n";
            return false;
        }
    }

    ostringstream q3;
    q3 << "SELECT h.seat_count FROM screening_tb s "
        << "JOIN hall_tb h ON s.hall_id = h.hall_id "
        << "WHERE s.screening_id = " << b.screening_id;
    MYSQL_RES* res3 = db.queryResult(q3.str());
    if (!res3) return false;
    MYSQL_ROW row3 = mysql_fetch_row(res3);
    int totalSeats = atoi(row3[0]);
    mysql_free_result(res3);
    if (b.seat_count <= 0 || b.seat_count > totalSeats) {
        cout << "❌ Invalid seat count.\n";
        return false;
    }

    ostringstream q1;
    q1 << "UPDATE screening_tb SET available_seats = available_seats - " << b.seat_count
        << " WHERE screening_id = " << b.screening_id;
    if (!db.execute(q1.str())) {
        cout << "❌ Failed to update available seats.\n";
        return false;
    }

    ostringstream q2;
    q2 << "INSERT INTO booking_tb (customer_id, screening_id, seats, seat_count, total_price, status, qr_code) VALUES ("
        << b.customer_id << "," << b.screening_id << ",'" << b.seats << "'," << b.seat_count
        << "," << b.total_price << ",'PENDING','')";
    if (!db.execute(q2.str())) {
        cout << "❌ Failed to insert booking.\n";
        return false;
    }

    MYSQL* conn = db.getConnection();
    int last = (int)mysql_insert_id(conn);
    b.id = last;
    b.qr = generateQRCodeData(b.id);
    ostringstream q4;
    q4 << "UPDATE booking_tb SET qr_code = '" << b.qr << "' WHERE booking_id = " << b.id;
    db.execute(q4.str());

    cout << "✅ Booking created successfully!\n";
    cout << "🎟 QR Code for booking " << b.id << ":\n";
    printQRCodeToConsole(b.qr);

    ostringstream qinfo;
    qinfo << R"(
        SELECT m.title, c.name, c.address, s.show_date, s.show_time
        FROM screening_tb s
        JOIN movie_tb m ON s.movie_id = m.movie_id
        JOIN cinema_tb c ON s.cinema_id = c.cinema_id
        WHERE s.screening_id = )" << b.screening_id;
    MYSQL_RES* info = db.queryResult(qinfo.str());
    if (!info) return true;
    MYSQL_ROW inf = mysql_fetch_row(info);
    if (!inf) { mysql_free_result(info); return true; }

    string movie = inf[0];
    string cinema = inf[1];
    string address = inf[2];
    string date = inf[3];
    string time = inf[4];
    mysql_free_result(info);

    ostringstream filename;
    filename << "Ticket_" << b.id << ".txt";
    ofstream file(filename.str());
    if (file.is_open()) {
        file << "=============================\n";
        file << "🎬 MOVIE TICKET\n";
        file << "Booking ID: " << b.id << "\n";
        file << "Movie: " << movie << "\n";
        file << "Cinema: " << cinema << "\n";
        file << "Address: " << address << "\n";
        file << "Date: " << date << "  Time: " << time << "\n";
        file << "Seats: " << b.seats << "\n";
        file << "Total: " << b.total_price << " VND\n";
        file << "Status: PENDING\n";
        file << "=============================\n";

        const QrCode qr = QrCode::encodeText(b.qr.c_str(), QrCode::Ecc::LOW);
        int border = 2;
        for (int y = -border; y < qr.getSize() + border; y++) {
            for (int x = -border; x < qr.getSize() + border; x++) {
                file << (qr.getModule(x, y) ? "██" : "  ");
            }
            file << "\n";
        }
        file.close();
        cout << "📄 Ticket saved as " << filename.str() << endl;
    }

    return true;
}

// 🟩 Hiển thị danh sách vé đã đặt
void Booking::showMyBookings(Database& db, int customer_id) {
    ostringstream q;
    q << "SELECT b.booking_id, m.title, s.show_date, s.show_time, b.seats, b.total_price, b.status "
      << "FROM booking_tb b "
      << "JOIN screening_tb s ON b.screening_id = s.screening_id "
      << "JOIN movie_tb m ON s.movie_id = m.movie_id "
      << "WHERE b.customer_id = " << customer_id;

    MYSQL_RES* res = db.queryResult(q.str());
    if (!res) return;

    cout << "\n--- My Bookings ---\n";
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        cout << "Booking ID: " << row[0] << "\n";
        cout << "Movie: " << row[1] << "\n";
        cout << "Date: " << row[2] << "\n";
        cout << "Time: " << row[3] << "\n";
        cout << "Seats: " << row[4] << "\n";
        cout << "Total: " << row[5] << "\n";
        cout << "Status: " << row[6] << "\n";
        cout << "-----------------------------\n";
    }
    mysql_free_result(res);
}
