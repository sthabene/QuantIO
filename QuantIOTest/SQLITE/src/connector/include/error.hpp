#pragma once
#include <stdexcept>
#include <string>
#include "sqlite3.h"

namespace QuantIO {
    /// Common data for SQL errors (abstract base)
    // ideally, this would be a concrete class deriving from std::exception,
    // but then our derived exceptions below couldn't derive from std::runtime & logic_errors
    class Error {
    public:
        virtual ~Error() = 0;
        virtual const char * sql() const noexcept;      /// Get SQL code where error was thrown
        virtual int err_code() const noexcept;          /// Get sqlite3 error code
        virtual const char * err_str() const noexcept;  /// Get a description of the sqlite3 error code
        virtual const char * err_msg() const noexcept;  /// Get the sqlite3 error message

    protected:
        /// sql Last SQL code ran
        /// sqlite_error_code Sqlite3 extended error code
        /// db Sqlite3 DB object
        Error(const std::string & sql, int sqlite_error_code, sqlite3 * db);

    private:
        std::string sql_;           ///< SQL code associated with the error
        int sqlite_error_code_;     ///< Sqlite3 extended error code
        sqlite3 * db_;              ///< Sqlite3 DB object
    };

    /// SQL Logic error
    class Logic_error: public virtual std::logic_error, public Error {
    public:
        /// what %Error message
        /// sql Last SQL code ran
        /// sqlite_error_code Sqlite3 extended error code
        /// db Sqlite3 DB object
        Logic_error(const std::string & what, const std::string & sql, int sqlite_error_code, sqlite3 * db);
        virtual ~Logic_error() = default;
    };

    /// SQL Runtime error
    class Runtime_error: public virtual std::runtime_error, public Error {
    public:
        /// what %Error message
        /// sql Last SQL code ran
        /// sqlite_error_code Sqlite3 extended error code
        /// db Sqlite3 DB object
        Runtime_error(const std::string & what, const std::string & sql, int sqlite_error_code, sqlite3 * db);
        virtual ~Runtime_error() = default;
    };
};
