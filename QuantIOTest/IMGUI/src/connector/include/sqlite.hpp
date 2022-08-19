#pragma once
#include <string>
#include <vector>
#include "sqlite3.h"
#include "imgui.h"

namespace QuantIO {
    /// Sqlite database connection
    /// Holds a connection to a database. SQL may be run with either the exec() method,
    /// or by using create_statement() to build a Connection::Stmt object
    class Connection final {
    public:
        class Stmt;
        explicit Connection(const std::string & filename);
        ~Connection();
        
        Connection(const Connection &) = delete; // non-copyable
        Connection & operator=(const Connection &) = delete;

        Connection(Connection &&);// movable
        Connection & operator=(Connection &&);

        Stmt create_statement(const std::string & sql); /// Create a new prepared statement

        /// Execute SQL statement(s)
        /// Will execute the SQL in-place, without needing to create a Connection::Stmt object.
        void exec(const std::string & sql, int (*callback)(void *, int, char **, char **) = nullptr, void * arg = nullptr);

        /// Start a transaction
        void begin_transaction();

        /// End a transaction & commit
        void commit();

        /// End a transaction & rollback
        void rollback();

        /// Interrupt a long-running query
        void interrupt();

        /// Get last INSERTed Row ID
        sqlite3_int64 last_insert_rowid();

        /// Get total number of rows modified
        /// Total number of rows inserted, modified or deleted by all
        /// INSERT, UPDATE or DELETE statements completed since the database
        /// connection was opened, including those executed as part of trigger
        /// programs
        int total_changes();

        /// Column metadata info
        /// This is the return type for table_column_metadata()
        struct Column_metadata {
            std::string type;           ///< Column's declared data type
            std::string collation;      ///< Name of default collation sequence
            bool not_null = false;      ///< \c true if the column has a NOT NULL constraint
            bool primary_key = false;   ///< \c true if column is part of the PRIMARY KEY
            bool auto_inc = false;      ///< \c true True if column is AUTOINCREMENT
        };

        /// Get column metadata
        Column_metadata table_column_metadata(
            const std::string & table_name, const std::string & column_name, const std::string & db_name = "main");

        /// Get wrapped C sqlite3 object
        const sqlite3 * get_c_obj() const;

        /// Get wrapped C sqlite3 object
        sqlite3 * get_c_obj();

        //Used to obtain table data
        std::vector<std::vector<std::string>> getTableData2(const std::string& sql, bool header = true,
            bool transpose = false);

    private:
        /// sqlite C API's DB connection obj
        sqlite3 * db_ = nullptr;
    };

    /// Prepared statement obj - usually created by Connection::create_statement
    /// Data is bound to the statement with the bind() overloads, step() is called
    /// to execute the statement, and then get_col() can be used to retrieve rows from a SELECT statement.
    /// When INSERTing or UPDATing multiple rows, call reset() to reuse the statement obj.
    class Connection::Stmt final {
    public:
        /// Prepare a new statement for the given SQL
        /// It is usually easier to use Connection::create_statement instead of this
        Stmt(const std::string & sql, Connection & db);
        Stmt(Connection& db);
        ~Stmt();

        Stmt(const Stmt &) = delete; // non-copyable
        Stmt & operator=(const Stmt &) = delete;

        Stmt(Stmt &&); // movable
        Stmt & operator=(Stmt &&);

        std::vector<std::vector<std::string>> getTableData(const std::string& sql);

        /// Functions for binding data to SQL statements
        /// Bind var by index
        /// As in the sqlite C API, bind var indexes start at 1
        /// index Bind variable index
        /// val Bind variable value
        void bind(const int index, const double val);
        void bind(const int index, const int val);
        void bind(const int index, const sqlite3_int64 val);
        void bind(const int index, const std::string & val);
        void bind(const int index, const char * val);
        void bind(const int index);
        void bind_null(const int index);

        /// Bind var by name
        /// name Bind variable name
        /// val Bind variable value
        void bind(const std::string & name, const double val);
        void bind(const std::string & name, const int val);
        void bind(const std::string & name, const sqlite3_int64 val);
        void bind(const std::string & name, const std::string & val);
        void bind(const std::string & name, const char * val);
        void bind(const std::string & name);
        void bind_null(const std::string & name); /// Bind null by name

        /// Get bind var name from index
        /// index Bind variable index
        /// returns Bind variable name
        std::string bind_parameter_name(const int index);

        /// Get bind var index by name
        /// param name Bind variable name
        /// returns Bind variable index
        int bind_parameter_index(const std::string & name);

        /// Get number of bind parameters
        /// returns Number of bind parameters
        int bind_parameter_count();


        /// Get number of columns
        int get_col_count();

        /// Get column name
        std::string get_col_name(int index);

        /// Run the statement.
        /// true on SELECT statements when more rows remain to be fetched
        /// false for UPDATE, DELETE, or database commands, or when no more rows can be SELECTED
        bool step();

        /// Get SELECTed column

        /// column Column number
        /// Unlike bind() indexes, column indexes start at 0
        /// returns Column data for the current row
        /// note Only specific template types are allowed
        /// See documentation for template specializations.
        template<typename T>
        T get_col(const int column);

        /// Reset the statement
        /// Useful for inserting or updating multiple rows
        void reset();

        /// Clear all bind vars to NULL
        void clear_bindings();

        /// returns true if the statement is busy (step has been called, but is not complete, nor reset)
        bool busy();

        /// returns true if the statement does not directly write to the DB
        bool readonly();

        /// returns C sqlite3_stmt object
        const sqlite3_stmt * get_c_obj() const;

        /// returns C sqlite3_stmt object
        sqlite3_stmt * get_c_obj();

    private:
        /// Sqlite C API's prepared statement obj
        /// Copy of sqlite DB connection obj
        sqlite3_stmt * stmt_ = nullptr;
        sqlite3 * db_ = nullptr;
    };
};
