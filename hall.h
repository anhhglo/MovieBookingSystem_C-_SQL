#ifndef HALL_H
#define HALL_H

#include <string>
#include "Database.h"

class Hall {
public:
    int hall_id;
    int cinema_id;
    std::string hall_name;
    int seat_count;

    Hall();
    Hall(int id, int cid, const std::string& name, int seats);

    static bool addHall(Database& db, const Hall& h);
    static void listHalls(Database& db);
    static void manageHalls(Database& db);
};

#endif
