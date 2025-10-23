#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include "Database.h"

class Booking {
public:
    int id;
    int customer_id;
    int screening_id;
    std::string seats; // "A1,A2"
    int seat_count;
    double total_price;
    std::string status; // PENDING/PAID/CANCELLED
    std::string qr;

    Booking();
    Booking(int customer_id, int screening_id, const std::string& seats, int seat_count, double total_price);

    static bool createBooking(Database& db, Booking& b);
    static bool markPaid(Database& db, int booking_id);
    static void showBooking(Database& db, int booking_id);
    static std::string generateQRCodePlaceholder(int booking_id);
};

#endif
