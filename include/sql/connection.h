/**
 * @file connection.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the ConnectionBase class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef SQL_CONNECTION_H
#define SQL_CONNECTION_H

#include <memory>

#include "prepare_statment.h"
#include "resultset.h"
#include "savepoint.h"
#include "statement.h"

namespace sql {

/**
 * @brief Abstract base class for database connections.
 *
 * This class provides a pure virtual interface for various database
 * connections. It is designed to be used by derived classes that provide
 * specific implementations for different database management systems.
 */
class ConnectionBase {
   public:
    virtual ~ConnectionBase() = default;

    /**
     * @brief Get the auto commit status of the connection.
     *
     * This method returns true if the connection is in auto commit mode,
     * false otherwise.
     *
     * @return true if the connection is in auto commit mode, false otherwise.
     */
    virtual bool auto_commit() = 0;

    /**
     * @brief Set the auto commit status of the connection.
     *
     * This method sets the auto commit status of the connection to the
     * specified value.
     *
     * @param auto_commit The auto commit status to be set.
     */
    virtual void set_auto_commit(bool auto_commit) = 0;

    /**
     * @brief Create a SQL statement object.
     *
     * This method creates a SQL statement object that can be used to execute
     * queries.
     *
     * @return A pointer to the statement object.
     */
    virtual Statement CreateStatement() = 0;

    /**
     * @brief Prepare a SQL statement.
     *
     * This method prepares a SQL statement for execution by binding parameters
     * to it.
     *
     * @param sql The SQL statement to be prepared.
     * @return A pointer to the prepared statement object.
     */
    virtual PreparedStatement PrepareStatement(const std::string& sql) = 0;

    /**
     * @brief Commit the current transaction.
     *
     * This method commits the current transaction, making all changes
     * permanent.
     */
    virtual void Commit() = 0;

    /**
     * @brief Roll back the current transaction to a savepoint.
     *
     * This method rolls back the current transaction to the specified
     * savepoint, discarding all changes made since the savepoint was created.
     *
     * @param savepoint The savepoint to roll back to.
     */
    virtual void Rollback(Savepoint& savepoint) = 0;

    /**
     * @brief Roll back the current transaction.
     *
     * This method rolls back the current transaction, discarding all changes
     * made since the transaction was started.
     */
    virtual void Rollback() = 0;

    /**
     * @brief Release a savepoint.
     *
     * This method releases a savepoint, making it unavailable for future use.
     *
     * @param savepoint The savepoint to release.
     */
    virtual void ReleaseSavepoint(Savepoint& savepoint) = 0;

    /**
     * @brief Create a savepoint.
     *
     * This method creates a savepoint, which can be used to roll back the
     * current transaction to a specific point.
     *
     * @param name The name of the savepoint to be created.
     * @return A pointer to the savepoint object.
     */
    virtual Savepoint SetSavepoint(const std::string& name) = 0;
};

using Connection = std::unique_ptr<ConnectionBase>;
}  // namespace sql
#endif
