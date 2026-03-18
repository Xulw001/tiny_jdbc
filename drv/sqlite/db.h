/**
 * @file db.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief SQLite database wrapper
 * @version 0.2
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef DRV_SQLITE_DB_H
#define DRV_SQLITE_DB_H

#include <sqlite3.h>

#include <memory>

#include "stmt.h"

namespace sql {
/**
 * @class SqliteDB
 * @brief SQLite database wrapper
 * @details Wraps sqlite3 database connection and provides helper methods
 */
class SqliteDB {
   public:
    /**
     * @brief Open a SQLite database
     * @param filename Database file name
     * @return Database object
     */
    static SqliteDB Open(const char* filename);

    /**
     * @brief Prepare a SQL statement
     * @param sql SQL statement string
     * @return Prepared statement object
     */
    SqliteStmt Prepare(const char* sql);

    /**
     * @brief Execute a SQL statement
     * @param sql SQL statement string
     */
    void Execute(const char* sql);

    /**
     * @brief Check for SQLite errors
     * @details Throws SqliteException if an error occurs
     */
    void Error();

    SqliteDB(const SqliteDB&) = delete;
    SqliteDB& operator=(const SqliteDB&) = delete;
    SqliteDB(SqliteDB&&) = default;
    SqliteDB& operator=(SqliteDB&&) = default;

   private:
    /**
     * @brief Construct a new SqliteDB object
     * @param db SQLite database handle
     */
    explicit SqliteDB(sqlite3* db) : db_(db, sqlite3_close) { ; }

   private:
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)>
        db_;  ///< The database connection
};
}  // namespace sql

#endif