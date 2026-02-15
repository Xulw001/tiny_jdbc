/**
 * @file stmt.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the SqliteStmt class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef DRV_SQLITE_STMT_H
#define DRV_SQLITE_STMT_H

#include <sqlite3.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "pointer.h"
#include "sql/value.h"

namespace sql {

using Value = std::vector<std::pair<ValueType, reflect::Pointer>>;

/**
 * @class SqliteStmt
 *
 * This class provides some utility functions to work with the sqlite3_stmt
 * object.
 *
 * @brief This class is used to wrap a sqlite3_stmt object.
 */
class SqliteStmt {
   public:
    /**
     * @brief Constructor.
     * @param stmt The sqlite3_stmt object to be wrapped.
     */
    explicit SqliteStmt::SqliteStmt(sqlite3_stmt* stmt)
        : stmt_(stmt, sqlite3_finalize) {}

    SqliteStmt(const SqliteStmt&) = delete;
    SqliteStmt& operator=(const SqliteStmt&) = delete;

    SqliteStmt(SqliteStmt&&) = default;
    SqliteStmt& operator=(SqliteStmt&&) = default;

    int Step();
    bool Bind(int index, reflect::ObjectInternal& obj, ValueType type);
    bool Value(int index, reflect::ObjectInternal& obj, ValueType type);
    void UpdateColumn();
    int GetIndexByName(const std::string& col_name);
    int column_bind_count();
    int column_count() { return col_count_; }

   private:
    void TypeCheck(int col_type, ValueType type);

   protected:
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>
        stmt_;                                      ///< sqlite3_stmt
    int col_count_;                                 ///< column count
    std::unordered_map<std::string, int> col_map_;  ///< column map
};

}  // namespace sql

#endif