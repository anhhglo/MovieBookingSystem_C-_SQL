#include "Booking.h"
#include <sstream>
#include <iostream>
#include <iomanip>

Booking::Booking() : id(0), customer_id(0), screening_id(0), seat_count(0), total_price(0.0), status("PENDING") {}
Booking::Booking(int customer_id, int screening_id, const std::string& seats, int seat_count, double total_price)
: customer_id(customer_id), screening_id(screening_id), seats(seats), seat_count(seat_count), total_price(total_price), status("PENDING") {}

bool Booking::createBooking(Database& db, Booking& b) {
    // 1) giảm available_seats trong screening_tb
    std::ostringstream q1;
    q1 << "UPDATE screening_tb SET available_seats = available_seats - " << b.seat_count << " WHERE screening_id = " << b.screening_id;
    if (!db.execute(q1.str())) return false;

    // 2) insert booking
    std::ostringstream q2;
    q2 << "INSERT INTO booking_tb (customer_id, screening_id, seats, seat_count, total_price, status, qr_code) VALUES ("
       << b.customer_id << "," << b.screening_id << ",'" << b.seats << "'," << b.seat_count << "," << b.total_price << ",'PENDING','')";
    if (!db.execute(q2.str())) return false;

    // 3) lấy id vừa insert
    MYSQL* conn = db.getConnection();
    int last = (int)mysql_insert_id(conn);
    b.id = last;

    // tạo qr code placeholder và update
    b.qr = generateQRCodePlaceholder(b.id);
    std::ostringstream q3;
    q3 << "UPDATE booking_tb SET qr_code = '" << b.qr << "' WHERE booking_id = " << b.id;
    db.execute(q3.str());
    return true;
}

bool Booking::markPaid(Database& db, int booking_id) {
    std::ostringstream q;
    q << "UPDATE booking_tb SET status='PAID' WHERE booking_id = " << booking_id;
    return db.execute(q.str());
}

void Booking::showBooking(Database& db, int booking_id) {
    std::ostringstream q;
    q << "SELECT b.booking_id, c.name, m.title, b.seats, b.total_price, b.status, b.qr_code, b.booked_at "
      << "FROM booking_tb b "
      << "JOIN customer_tb c ON b.customer_id = c.customer_id "
      << "JOIN screening_tb s ON b.screening_id = s.screening_id "
      << "JOIN movie_tb m ON s.movie_id = m.movie_id "
      << "WHERE b.booking_id = " << booking_id;
    MYSQL_RES* res = db.queryResult(q.str());
    if (!res) return;
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(res))) {
        std::cout << "Booking ID: " << row[0] << "\nCustomer: " << row[1] << "\nMovie: " << row[2] << "\nSeats: " << row[3]
                  << "\nTotal: " << row[4] << "\nStatus: " << row[5] << "\nQR: " << row[6] << "\nBooked at: " << row[7] << std::endl;
    } else {
        std::cout << "Not found.\n";
    }
    mysql_free_result(res);
}

std::string Booking::generateQRCodePlaceholder(int booking_id) {
    // placeholder string, thực tế bạn dùng thư viện QR code để tạo ảnh hoặc data URI
    std::ostringstream s;
    s << "QRBOOKING-" << booking_id << "-TOKEN";
    return s.str();
}
