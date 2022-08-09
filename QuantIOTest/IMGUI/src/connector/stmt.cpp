
#include "include/sqlite.hpp"
#include "include/error.hpp"

using namespace std::string_literals;

namespace QuantIO {
    Connection::Stmt::Stmt(const std::string & sql, Connection & db):
        db_(db.get_c_obj()) {
        int status = sqlite3_prepare_v2(db.get_c_obj(), sql.c_str(), sql.length() + 1, &stmt_, NULL);
        if(status != SQLITE_OK) {
            throw Logic_error("Error parsing SQL: "s + sqlite3_errmsg(db.get_c_obj()), sql, status, db_);
        }
    }

    Connection::Stmt::Stmt(Connection& db) : db_(db.get_c_obj()) {

    }

    Connection::Stmt::~Stmt() {
        sqlite3_finalize(stmt_);
    }

    Connection::Stmt::Stmt(Connection::Stmt && other): stmt_{other.stmt_}, db_{other.db_} {
        other.stmt_ = nullptr;
    }

    Connection::Stmt & Connection::Stmt::operator=(Connection::Stmt && other) {
        if(&other != this) {
            sqlite3_finalize(stmt_);
            stmt_ = other.stmt_;
            db_ = other.db_;
            other.stmt_ = nullptr;
        }
        return *this;
    }

    void Connection::Stmt::bind(const int index, const double val) {
        int status = sqlite3_bind_double(stmt_, index, val);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding index " +
                std::to_string(index) + ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind(const int index, const int val) {
        int status = sqlite3_bind_int(stmt_, index, val);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding index " +
                std::to_string(index) + ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind(const int index, const sqlite3_int64 val) {
        int status = sqlite3_bind_int64(stmt_, index, val);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding index " +
                std::to_string(index) + ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind(const int index, const std::string & val) {
        int status = sqlite3_bind_text(stmt_, index, val.c_str(), val.length(), SQLITE_TRANSIENT);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding index " +
                std::to_string(index) + ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind(const int index, const char * val) {
        int status = sqlite3_bind_text(stmt_, index, val, -1, SQLITE_TRANSIENT);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding index " +
                std::to_string(index) + ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind_null(const int index) {
        int status = sqlite3_bind_null(stmt_, index);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding index " +
                std::to_string(index) + ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind(const int index) {
        bind_null(index);
    }

    void Connection::Stmt::bind(const std::string & name, const double val) {
        int status = sqlite3_bind_double(stmt_, bind_parameter_index(name), val);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding " + name +
                ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind(const std::string & name, const int val) {
        int status = sqlite3_bind_int(stmt_, bind_parameter_index(name), val);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding " + name +
                ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind(const std::string & name, const sqlite3_int64 val) {
        int status = sqlite3_bind_int64(stmt_, bind_parameter_index(name), val);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding " + name +
                ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind(const std::string & name, const std::string & val) {
        int status = sqlite3_bind_text(stmt_, bind_parameter_index(name), val.c_str(), val.length(), SQLITE_TRANSIENT);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding " + name +
                ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind(const std::string & name, const char * val) {
        int status = sqlite3_bind_text(stmt_, bind_parameter_index(name), val, -1, SQLITE_TRANSIENT);
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding " + name +
                ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind_null(const std::string & name) {
        int status = sqlite3_bind_null(stmt_, bind_parameter_index(name));
        if(status != SQLITE_OK) {
            throw Logic_error("Error binding " + name +
                ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::bind(const std::string & name) {
        bind_null(name);
    }

    std::string Connection::Stmt::bind_parameter_name(const int index) {
        const char * name = sqlite3_bind_parameter_name(stmt_, index);
        if(!name) {
            throw Logic_error("Error looking up bind var name for index " +
                std::to_string(index) + ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), SQLITE_ERROR, db_);
        }
        return std::string(name);
    }

    int Connection::Stmt::bind_parameter_index(const std::string & name) {
        int index = sqlite3_bind_parameter_index(stmt_, name.c_str());
        if(!index) {
            throw Logic_error("Error looking up index for bind var " +
                name + ": " + sqlite3_errmsg(db_), sqlite3_sql(stmt_), SQLITE_ERROR, db_);
        }
        return index;
    }

    int Connection::Stmt::bind_parameter_count() {
        return sqlite3_bind_parameter_count(stmt_);
    }

    int Connection::Stmt::get_col_count() {
        return sqlite3_column_count(stmt_);
    }

    std::string Connection::Stmt::get_col_name(int index) {
        std::string col = sqlite3_column_name(stmt_, index);
        return col;
    }

    bool Connection::Stmt::step() {
        int status = sqlite3_step(stmt_);
        if(status == SQLITE_ROW) {
            return true;
        }
        else if(status == SQLITE_DONE) {
            return false;
        }
        else {
            throw Logic_error("Error evaluating SQL: "s +
                sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    /// @name Get columns
    /// get_col() template specializations
    /// @{

    /// @copydoc sqlite::Connection::Stmt::get_col()
    template<>
    double Connection::Stmt::get_col<double>(const int column) {
        return sqlite3_column_double(stmt_, column);
    }

    /// @copydoc sqlite::Connection::Stmt::get_col()
    template<>
    int Connection::Stmt::get_col<int>(const int column) {
        return sqlite3_column_int(stmt_, column);
    }

    /// @copydoc sqlite::Connection::Stmt::get_col()
    template<>
    sqlite3_int64 Connection::Stmt::get_col<sqlite3_int64>(const int column) {
        return sqlite3_column_int64(stmt_, column);
    }

    /// @copydoc sqlite::Connection::Stmt::get_col()
    /// @memberof sqlite::Connection::Stmt
    template<>
    std::string Connection::Stmt::get_col<std::string>(const int column) {
        const char * str = reinterpret_cast<const char *>(sqlite3_column_text(stmt_, column));

        if(!str)
            return ""s; // empty str for NULL data
        else
            return std::string(str);
    }

    /// @copydoc sqlite::Connection::Stmt::get_col()
    template<>
    const char * Connection::Stmt::get_col<const char *>(const int column) {
        return reinterpret_cast<const char *>(sqlite3_column_text(stmt_, column));
    }

    /// @}

    void Connection::Stmt::reset() {
        int status = sqlite3_reset(stmt_);
        if(status != SQLITE_OK) {
            throw Logic_error("Error resetting statement: "s +
                sqlite3_errmsg(db_), sqlite3_sql(stmt_), status, db_);
        }
    }

    void Connection::Stmt::clear_bindings() {
        sqlite3_clear_bindings(stmt_);
    }

    bool Connection::Stmt::busy() {
        return bool(sqlite3_stmt_busy(stmt_));
    }

    bool Connection::Stmt::readonly() {
        return bool(sqlite3_stmt_readonly(stmt_));
    }

    const sqlite3_stmt * Connection::Stmt::get_c_obj() const {
        return stmt_;
    }

    std::vector<std::vector<std::string>> Connection::Stmt::getTableData(const std::string& sql) {
        printf("%s\n", sql.c_str());
        std::vector<std::vector<std::string>> tableVec;

        sqlite3_prepare_v2(db_, sql.c_str(), sql.length() + 1, &stmt_, NULL);
        std::vector<std::string> v0; //Header
        static const int columns = this->get_col_count();
        for (int column = 0; column < columns; column++) {
            v0.push_back(this->get_col_name(column));
        }
        tableVec.push_back(v0);

        std::string data;
        while (this->step()) {
            std::vector<std::string> v1;
            for (int column = 0; column < columns; column++) {
                data = this->get_col<std::string>(column);
                v1.push_back( data.empty() ? "" : data);
            }
            tableVec.push_back(v1);
        }
        this->reset();
        return tableVec;
    }
};
