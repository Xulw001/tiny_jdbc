/**
 * @file statement.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief SQLite implementation of statement
 * @version 0.2
 * @date 2026-03-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef DRV_SQLITE_STATEMENT_H
#define DRV_SQLITE_STATEMENT_H

#include <vector>

#include "connection.h"
#include "sql/statement.h"
#include "stmt.h"

namespace sql {
/**
 * @class SqliteStatement
 * @brief SQLite implementation of StatementBase
 * @details Implements SQL statement operations for SQLite
 */
class SqliteStatement : public StatementBase {
   public:
    /**
     * @brief Construct a new SqliteStatement object
     * @param conn Reference to SqliteConnection
     */
    SqliteStatement(SqliteConnection& conn) : conn_(conn), stmt_(nullptr) { ; }

    virtual ~SqliteStatement() = default;

    /**
     * @brief Execute a SQL query and return a ResultSet
     * @param sql SQL query string
     * @return Reference to the result set containing query results
     */
    virtual ResultSet ExecuteQuery(const std::string& sql) override;

    /**
     * @brief Execute a SQL update statement
     * @param sql SQL update string
     */
    virtual void ExecuteUpdate(const std::string& sql) override;

    /**
     * @brief Get the current result set
     * @return Reference to the current result set
     */
    virtual ResultSet GetResultSet() override { return result_set_; }

    /**
     * @brief Add a SQL statement to the batch
     * @param sql SQL statement to add to the batch
     */
    virtual void AddBatch(const std::string& sql) override;

    /**
     * @brief Execute all statements in the batch
     */
    virtual void ExecuteBatch() override;

    /**
     * @brief Clear all statements in the batch
     */
    virtual void ClearBatch() override;

   private:
    SqliteConnection& conn_;               ///< Reference to SQLite connection
    SqliteStmt stmt_;                      ///< SQLite statement wrapper
    std::vector<std::string> batch_sqls_;  ///< Batch SQL statements
    std::unique_ptr<sql::ResultSetBase> result_set_ =
        nullptr;  ///< Current result set
};

}  // namespace sql

#endif