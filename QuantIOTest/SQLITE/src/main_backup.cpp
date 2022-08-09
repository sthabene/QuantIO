//Disable warnings
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <string>
#include "sqlite3.h"



int main() {

    char query[999];
    std::string query2;
    sqlite3* db;
    sqlite3_stmt* stmt;

    query[sizeof(query) - 1] = '\0';
    snprintf(query, sizeof(query) - 1, "SELECT CODE FROM CURRENCY");
    query2 = "SELECT CODE FROM CURRENTY";

    if (sqlite3_open("..\\..\\misc\\database\\main.db", &db) != SQLITE_OK) { //Open a connection to a an SQLite database
        fprintf(stderr, "Error opening database.\n");
    }

    sqlite3_prepare(db, query, sizeof(query), &stmt, NULL); //creates prepared statement
    //sqlite3_prepare(db, query2.c_str(), query2.size(), &stmt, NULL); //creates prepared statement

    bool done = false;
    while (!done) {
        switch (sqlite3_step(stmt)) {
        case SQLITE_ROW:
            printf("%s\n", sqlite3_column_text(stmt, 0));
            break;
        case SQLITE_DONE:
            done = true;
            break;
        default:
            fprintf(stderr, "Failed.\n");
            return 1;
        }
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return 0;
}