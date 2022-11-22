#pragma once
#include "sqlite.hpp"

namespace QuantIO {
    class Database final { // final means it cannot be derived
    public:
        
        Database() = delete; // make non creatable
        ~Database() = delete;

        static Connection& get();  //Get DB connection
        static void init(Connection* db);  // Set DB connection

    private:
        static Connection* db_; // DB connection
    };
};
