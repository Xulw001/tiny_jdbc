/**
 * @file statement.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the SqliteStatement class
 * @version 0.1
 * @date 2026-02-14
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
 *
 * This class provides a RAII interface to a sqlite3 database connection. It is
 * used to execute SQL queries and updates, and to retrieve the result set of a
 * query.
 *
 * @brief A class that provides a RAII interface to a sqlite3 database
 *        connection.
 * @see StatementBase
 */
class SqliteStatement : public StatementBase {
   public:
    /**
     * @brief Constructor.
     *
     * @param conn The database connection object.
     */
    SqliteStatement(SqliteConnection& conn) : conn_(conn), stmt_(nullptr) { ; }
    virtual ~SqliteStatement() = default;

    virtual ResultSet& ExecuteQuery(const std::string& sql) override;
    virtual void ExecuteUpdate(const std::string& sql) override;
    virtual ResultSet& GetResultSet() override { return result_set_; }

    virtual void AddBatch(const std::string& sql) override;
    virtual void ExecuteBatch() override;
    virtual void ClearBatch() override;

   private:
    SqliteConnection& conn_;
    SqliteStmt stmt_;
    std::vector<std::string> batch_sqls_;
    ResultSet result_set_ = nullptr;
};

}  // namespace sql

#endif