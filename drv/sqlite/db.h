/**
 * @file db.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the SqliteDB class
 * @version 0.1
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
 *
 * This class provides a RAII interface to a sqlite3 database connection.
 * It also provides some utility functions to prepare and execute SQL
 * statements.
 *
 * @brief A class that wraps a sqlite3 database connection.
 */
class SqliteDB {
   public:
    static SqliteDB Open(const char* filename);
    SqliteStmt Prepare(const char* sql);
    void Execute(const char* sql);
    void Error();

    SqliteDB(const SqliteDB&) = delete;
    SqliteDB& operator=(const SqliteDB&) = delete;

    SqliteDB(SqliteDB&&) = default;
    SqliteDB& operator=(SqliteDB&&) = default;

   private:
    /*
     * @brief Constructor that takes a sqlite3 database connection.
     *
     * @param db The database connection to take ownership of.
     */
    explicit SqliteDB(sqlite3* db) : db_(db, sqlite3_close) { ; }

   private:
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)>
        db_;  ///< The database connection
};
}  // namespace sql

#endif