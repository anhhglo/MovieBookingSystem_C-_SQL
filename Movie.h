#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include "Database.h"

class Movie {
public:
    int id;
    std::string title;
    std::string genre;
    int duration_minutes;
    std::string description;

    Movie();
    Movie(int id, const std::string& title, const std::string& genre, int dur, const std::string& desc);

    // DB operations
    static bool addMovie(Database& db, const Movie& m);
    static void listMovies(Database& db);

    // ✅ Thêm dòng này
    static void manageMovies(Database& db);
};

#endif
