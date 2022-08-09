#include "../include/database.hpp"
#include <stdexcept>

namespace QuantIO {
    Connection & Database::get() {
        if(!db_)
            throw std::logic_error("Database not initialized");
        return *db_;
    }

    void Database::init(Connection * db) {
        db_ = db;
    }

    // init to null
    Connection * Database::db_ = nullptr;
};
