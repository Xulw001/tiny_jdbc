/**
 * @file stmt.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief SQLite statement wrapper for prepared statements
 * @version 0.2
 * @date 2026-03-14
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

#include "sql/value.h"
#include "type/reference.h"
#include "type/value.h"

namespace sql {
using namespace reflect;
/**
 * @class SqliteStmt
 * @brief SQLite statement wrapper
 * @details Wraps sqlite3_stmt and provides helper methods
 *          for statement operations
 */
class SqliteStmt {
   public:
    /**
     * @brief Construct a new SqliteStmt object
     * @param stmt SQLite statement handle
     */
    explicit SqliteStmt(sqlite3_stmt* stmt) : stmt_(stmt, sqlite3_finalize) {}

    SqliteStmt(const SqliteStmt&) = delete;
    SqliteStmt& operator=(const SqliteStmt&) = delete;

    SqliteStmt(SqliteStmt&&) = default;
    SqliteStmt& operator=(SqliteStmt&&) = default;

    /**
     * @brief Execute the statement step
     * @return SQLite result code
     */
    int Step();

    /**
     * @brief Bind a parameter to the statement
     * @param index Parameter index (1-based)
     * @param ref Reference to the value to bind
     * @return True if binding successful, false otherwise
     */
    bool BindParameter(int index, Reference ref);

    /**
     * @brief Get a value from the current row
     * @param index Column index (0-based)
     * @param type Expected value type
     * @return Retrieved value
     */
    Value GetValue(int index, ValueType type);

    /**
     * @brief Update column information
     */
    void UpdateColumn();

    /**
     * @brief Get column index by name
     * @param col_name Column name
     * @return Column index (0-based), or -1 if not found
     */
    int GetIndexByName(const std::string& col_name);

    /**
     * @brief Get the number of parameters to bind
     * @return Number of parameters
     */
    int column_bind_count();

    /**
     * @brief Get the number of columns in the result set
     * @return Number of columns
     */
    int column_count() { return col_count_; }

   private:
    /**
     * @brief Check if the column type matches the expected type
     * @param col_type SQLite column type
     * @param type Expected value type
     */
    void TypeCheck(int col_type, ValueType type);

   protected:
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>
        stmt_;       ///< SQLite statement handle
    int col_count_;  ///< Number of columns in result set
    std::unordered_map<std::string, int>
        col_map_;  ///< Map of column names to indices
};

}  // namespace sql

#endif