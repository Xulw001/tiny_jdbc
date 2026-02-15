#include "db.h"

#include "sql/exception.h"

namespace sql {

/**
 * @brief Open a SQLite database file.
 *
 * The filename may be a path relative to the current working
 * directory, or an absolute path. If the file does not exist, it
 * will be created.
 *
 * @param filename The name of the database file to open.
 * @return A SqliteDB object representing the open database.
 * @throws SQLException If the database cannot be opened.
 */
SqliteDB SqliteDB::Open(const char* filename) {
    sqlite3* raw_db = nullptr;
    if (SQLITE_OK != sqlite3_open_v2(filename, &raw_db,
                                     SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                     NULL)) {
        throw SQLException("open sqlite database failed!");
    }
    return SqliteDB(raw_db);
}

/**
 * @brief Prepare a SQL statement.
 *
 * This function prepares a SQL statement for execution by compiling
 * the SQL text into a sqlite3_stmt object. The prepared statement
 * can be executed multiple times with different bind values.
 *
 * @param sql The SQL statement to be prepared.
 * @return A SqliteStmt object representing the prepared statement.
 * @throws SQLException If the statement cannot be prepared.
 */
SqliteStmt SqliteDB::Prepare(const char* sql) {
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw SQLException(sqlite3_errmsg(db_.get()));
    return SqliteStmt(stmt);
}

/**
 * @brief Execute a SQL statement.
 *
 * This function executes a SQL statement on the database. It does not
 * return any value, nor does it throw an exception if the statement
 * succeeds. If the statement fails, it throws a SQLException with a
 * message describing the error.
 *
 * @param sql The SQL statement to be executed.
 */
void SqliteDB::Execute(const char* sql) {
    if (sqlite3_exec(db_.get(), sql, nullptr, nullptr, nullptr) == SQLITE_OK)
        return;
    Error();
}

/**
 * @brief Throw an exception if there is an error.
 *
 * This function checks if there is an error in the database object
 * and throws a SQLException with a message describing the error if
 * there is one.
 */
void SqliteDB::Error() {
    if (sqlite3_errcode(db_.get())) {
        throw SQLException(sqlite3_errmsg(db_.get()));
    }
}

}  // namespace sql