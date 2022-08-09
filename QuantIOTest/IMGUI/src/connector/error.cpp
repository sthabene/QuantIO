#include "include/error.hpp"

namespace QuantIO {
    Error::~Error() {
    }

    const char * Error::sql() const noexcept {
        return sql_.c_str();
    }

    int Error::err_code() const noexcept {
        return sqlite_error_code_;
    }

    const char * Error::err_str() const noexcept {
        return sqlite3_errstr(err_code());
    }

    const char * Error::err_msg() const noexcept {
        if(!db_)
            return "";
        return sqlite3_errmsg(db_);
    }

    Error::Error(const std::string & sql, int sqlite_error_code, sqlite3 * db):
        sql_(sql),
        sqlite_error_code_(sqlite_error_code),
        db_(db) {
    }

    Logic_error::Logic_error(const std::string & what, const std::string & sql, int sqlite_error_code, sqlite3 * db):
        std::logic_error(what),
        Error(sql, sqlite_error_code, db) {
    }

    Runtime_error::Runtime_error(const std::string & what, const std::string & sql, int sqlite_error_code, sqlite3 * db):
        std::runtime_error(what),
        Error(sql, sqlite_error_code, db) {
    }
};
