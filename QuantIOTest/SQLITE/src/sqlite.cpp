#include "../include/sqlite.hpp"
#include "../include/error.hpp"

namespace QuantIO {
    Connection::Connection(const std::string & filename) {
        int status = sqlite3_open(filename.c_str(), &db_);
        if(status != SQLITE_OK) {
            sqlite3_close(db_);
            throw Runtime_error("Error connecting to db (" +
                filename + "): " + sqlite3_errmsg(db_), "", status, nullptr);
        }
        sqlite3_extended_result_codes(db_, true);
    }

    Connection::~Connection() {
        sqlite3_close(db_);
    }

    Connection::Connection(Connection && other): db_{other.db_} {
        other.db_ = nullptr;
    }

    Connection & Connection::operator=(Connection && other) {
        if(&other != this) {
            sqlite3_close(db_);
            db_ = other.db_;
            other.db_ = nullptr;
        }
        return *this;
    }

    Connection::Stmt Connection::create_statement(const std::string & sql) {
        return Stmt(sql, *this);
    }

  /*  int Connection::create_another_statement(const std::string& sql) {
        return sqlite3_prepare_v2(db_, sql.c_str(), -1, stmt_, NULL);
    };*/

    void Connection::exec(const std::string & sql, int (*callback)(void *, int, char **, char **), void * arg) {
        char * err_msg = nullptr;
        int status = sqlite3_exec(db_, sql.c_str(), callback, arg, &err_msg);

        if(err_msg || status != SQLITE_OK) {
            std::string err = "";
            if(err_msg) {
                err = err_msg;
                sqlite3_free(err_msg);
            }
            throw Logic_error("Error evaluating SQL: " + err, sql, status, db_);
        }
    }

    void Connection::begin_transaction() {
        exec("BEGIN TRANSACTION;");
    }

    void Connection::commit() {
        exec("COMMIT;");
    }

    void Connection::rollback() {
        exec("ROLLBACK;");
    }

    void Connection::interrupt() {
        sqlite3_interrupt(db_);
    }

    sqlite3_int64 Connection::last_insert_rowid() {
        return sqlite3_last_insert_rowid(db_);
    }

    int Connection::total_changes() {
        return sqlite3_total_changes(db_);
    }

    Connection::Column_metadata Connection::table_column_metadata(const std::string & table_name, const std::string & column_name,
        const std::string & db_name) {
        const char * type, * collation;
        int not_null, primary_key, auto_inc;

        int status = sqlite3_table_column_metadata(db_, db_name.c_str(), table_name.c_str(), column_name.c_str(),
            &type, &collation, &not_null, &primary_key, &auto_inc);

        if(status != SQLITE_OK) {
            sqlite3_close(db_);
            throw Runtime_error("Error getting column info (" +
                db_name + "." + table_name + "." + column_name + "): " +
                sqlite3_errmsg(db_), "", status, nullptr);
        }

        Column_metadata ret;
        ret.type = type;
        ret.collation = collation;
        ret.not_null = bool(not_null);
        ret.primary_key = bool(primary_key);
        ret.auto_inc = bool(auto_inc);

        return ret;
    }

    const sqlite3 * Connection::get_c_obj() const {
        return db_;
    }

    sqlite3 * Connection::get_c_obj() {
        return db_;
    }

    sqlite3_stmt * Connection::Stmt::get_c_obj() {
        return stmt_;
    }
};
