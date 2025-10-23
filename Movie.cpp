#include "Movie.h"
#include <sstream>
#include <iostream>

Movie::Movie() : id(0), duration_minutes(0) {}
Movie::Movie(int id, const std::string& title, const std::string& genre, int dur, const std::string& desc)
: id(id), title(title), genre(genre), duration_minutes(dur), description(desc) {}

bool Movie::addMovie(Database& db, const Movie& m) {
    std::ostringstream q;
    q << "INSERT INTO movie_tb (title, m_genre, duration_minutes, description) VALUES ("
      << "'" << m.title << "', '" << m.genre << "', " << m.duration_minutes << ", '" << m.description << "')";
    return db.execute(q.str());
}

void Movie::listMovies(Database& db) {
    MYSQL_RES* res = db.queryResult("SELECT movie_id, title, m_genre, duration_minutes FROM movie_tb");
    if (!res) return;
    MYSQL_ROW row;
    printf("-------------------------------------------------\n");
    printf("| %-5s | %-25s | %-10s | %-6s |\n", "ID", "Title", "Genre", "Dur");
    while ((row = mysql_fetch_row(res))) {
        printf("| %-5s | %-25s | %-10s | %-6s |\n", row[0], row[1], row[2], row[3]);
    }
    printf("-------------------------------------------------\n");
    mysql_free_result(res);
}

void Movie::manageMovies(Database& db) {
    int choice;
    while (true) {
        std::cout << "\n=== Manage Movies ===\n";
        std::cout << "1. List Movies\n2. Add Movie\n3. Back\nChoose: ";
        std::cin >> choice;
        if (choice == 1) {
            listMovies(db);
        } else if (choice == 2) {
            std::string title, genre, desc;
            int dur;
            std::cin.ignore();
            std::cout << "Title: "; getline(std::cin, title);
            std::cout << "Genre: "; getline(std::cin, genre);
            std::cout << "Duration (minutes): "; std::cin >> dur; std::cin.ignore();
            std::cout << "Description: "; getline(std::cin, desc);
            Movie m(0, title, genre, dur, desc);
            if (addMovie(db, m))
                std::cout << "Movie added.\n";
            else
                std::cout << "Failed to add movie.\n";
        } else if (choice == 3) break;
        else std::cout << "Invalid choice.\n";
    }
}

