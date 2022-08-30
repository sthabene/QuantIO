#include "include/sqlite.hpp"
#include "include/error.hpp"

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

    std::vector<std::vector<std::string>> Connection::getTableData2(const std::string& sql, bool header,
        bool transpose) {
        //Prepared statement
        Stmt statement(sql, *this);
        printf("%s;\n", sql.c_str());

        //Stores the table return data
        const std::size_t columns = statement.get_col_count();
        std::vector<std::vector<std::string>> tableVec;

        if (header) {
            std::vector<std::string> v0; //Header
            v0.reserve(columns);
            for (std::size_t column = 0; column < columns; column++) {
                v0.push_back(statement.get_col_name(column));
            }
            tableVec.push_back(v0);
        }

        std::string data;
        while (statement.step()) {
            std::vector<std::string> v1;
            v1.reserve(columns);
            for (std::size_t column = 0; column < columns; column++) {
                data = statement.get_col<std::string>(column);
                v1.push_back(data.empty() ? "" : data);
            }
            tableVec.push_back(v1);
        }
        statement.reset();

        if (transpose) {
            std::vector<std::vector<std::string>> tresult(
                tableVec[0].size(), std::vector<std::string>(tableVec.size())
            );
            for (std::vector<std::string>::size_type i = 0; i < tableVec[0].size(); i++) {
                for (std::vector<std::string>::size_type j = 0; j < tableVec.size(); j++) {
                    tresult[i][j] = tableVec[j][i];
                }
            }
            tableVec = tresult;
        }
        return tableVec;
    };


    void Connection::updateData(const std::string& sql) {
        //Prepared statement
        Stmt statement(sql, *this);
        printf("%s;\n", sql.c_str());
        statement.step();
        
        //this->commit();
        statement.reset();
    };

};
