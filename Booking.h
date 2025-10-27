#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include "Database.h"

class Booking {
public:
    int id;
    int customer_id;
    int screening_id;
    std::string seats;
    int seat_count;
    double total_price;
    std::string status;
    std::string qr;

    // Các hàm chính
    static bool createBooking(Database& db, Booking& b);
    static void showMyBookings(Database& db, int customer_id);
    static void showSeatMatrix(Database& db, int screening_id);

    // Tạo QR dạng text placeholder
    static std::string generateQRCodePlaceholder(int booking_id);

    // 🆕 Thêm 2 hàm này để khớp với phần định nghĩa trong Booking.cpp
    static std::string generateQRCodeData(int booking_id);
    static void printQRCodeToConsole(const std::string& text);
};

#endif
