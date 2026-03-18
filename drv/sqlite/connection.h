/**
 * @file connection.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief SQLite implementation of connection
 * @version 0.2
 * @date 2026-03-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef DRV_SQLITE_CONNECTION_H
#define DRV_SQLITE_CONNECTION_H

#include "db.h"
#include "sql/connection.h"

namespace sql {
/**
 * @class SqliteConnection
 * @brief SQLite implementation of ConnectionBase
 * @details Implements database connection operations for SQLite
 */
class SqliteConnection : public ConnectionBase {
    friend class SqliteStatement;
    friend class SqlitePreparedStatement;

   public:
    /**
     * @brief Construct a new SqliteConnection object
     * @param path Database file path
     * @param params Connection parameters
     */
    SqliteConnection(const char* path, const char* params);

    virtual ~SqliteConnection() { ; }

    /**
     * @brief Create a new statement
     * @return New statement object
     */
    virtual Statement CreateStatement() override;

    /**
     * @brief Create a new prepared statement
     * @param sql SQL statement string
     * @return New prepared statement object
     */
    virtual PreparedStatement PrepareStatement(const std::string& sql) override;

    /**
     * @brief Get the auto-commit mode
     * @return True if auto-commit is enabled, false otherwise
     */
    virtual bool auto_commit() override { return auto_commit_; }

    /**
     * @brief Set the auto-commit mode
     * @param auto_commit True to enable auto-commit, false to disable
     */
    virtual void set_auto_commit(bool auto_commit) override;

    /**
     * @brief Commit the current transaction
     */
    virtual void Commit() override;

    /**
     * @brief Rollback the current transaction
     */
    virtual void Rollback() override;

    /**
     * @brief Rollback to a savepoint
     * @param savepoint Savepoint to rollback to
     */
    virtual void Rollback(Savepoint& savepoint) override;

    /**
     * @brief Release a savepoint
     * @param savepoint Savepoint to release
     */
    virtual void ReleaseSavepoint(Savepoint& savepoint) override;

    /**
     * @brief Set a savepoint with a name
     * @param name Savepoint name
     * @return New savepoint object
     */
    virtual Savepoint SetSavepoint(const std::string& name) override;

    /**
     * @brief Check for SQLite errors
     * @details Throws SQLException if an error occurs
     */
    void Error();

   private:
    /**
     * @brief Ensure transaction commited if auto-commit is enabled
     */
    void EnsureAutoCommit();

   private:
    SqliteDB db_;              ///< SQLite database connection
    bool auto_commit_ = true;  ///< Auto-commit mode flag
};

}  // namespace sql
#endif