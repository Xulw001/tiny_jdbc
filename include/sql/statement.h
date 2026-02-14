/**
 * @file statement.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the Statement class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef SQL_STATEMENT_H
#define SQL_STATEMENT_H

#include "resultset.h"

namespace sql {

/**
 * @brief A base class for SQL statements.
 *
 * This class provides a common interface for various SQL statements, including
 * queries and updates. It is designed to be used by derived classes that
 * provide specific implementations for different database management systems.
 *
 * @see Statement
 */
class StatementBase {
   public:
    virtual ~StatementBase() = default;
    /**
     * @brief Executes a query and returns the result set.
     *
     * @param sql The SQL query to be executed.
     * @return A pointer to the result set.
     */
    virtual ResultSet& ExecuteQuery(const std::string& sql) = 0;
    /**
     * @brief Retrieves the result set of the statement.
     *
     * This method can be used to retrieve the result set after calling
     * ExecuteQuery.
     *
     * @return A pointer to the result set.
     */
    virtual ResultSet& GetResultSet() = 0;
    /**
     * @brief Executes an update statement.
     *
     * @param sql The SQL update statement to be executed.
     */
    virtual void ExecuteUpdate(const std::string& sql) = 0;
    /**
     * @brief Adds a SQL statement to the batch.
     *
     * @param sql The SQL statement to be added to the batch.
     */
    virtual void AddBatch(const std::string& sql) = 0;
    /**
     * @brief Executes the batch of SQL statements.
     */
    virtual void ExecuteBatch() = 0;
    /**
     * @brief Clears the batch of SQL statements.
     */
    virtual void ClearBatch() = 0;
};

using Statement = std::unique_ptr<StatementBase>;
}  // namespace sql

#endif