#ifndef CINEMA_H
#define CINEMA_H

#include <string>
#include "Database.h"

class Cinema {
public:
    int id;
    std::string name;
    std::string address;

    Cinema();
    Cinema(int id, const std::string& name, const std::string& address);

    static bool addCinema(Database& db, const Cinema& c);
    static void listCinemas(Database& db);

    // ✅ Thêm dòng này
    static void manageCinemas(Database& db);
};

#endif
