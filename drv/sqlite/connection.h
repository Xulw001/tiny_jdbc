/**
 * @file connection.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the SqliteConnection class
 * @version 0.1
 * @date 2026-02-14
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
 *
 * This class provides a RAII interface to a sqlite3 database connection.
 * It also provides some utility functions to prepare and execute SQL
 * statements.
 *
 * @brief A class that wraps a sqlite3 database connection.
 */
class SqliteConnection : public ConnectionBase {
    friend class SqliteStatement;
    friend class SqlitePreparedStatement;

   public:
    SqliteConnection(const char* path, const char* params);
    virtual ~SqliteConnection() { ; }

    virtual Statement CreateStatement() override;
    virtual PreparedStatement PrepareStatement(const std::string& sql) override;

    virtual bool auto_commit() override { return auto_commit_; }
    virtual void set_auto_commit(bool auto_commit) override;

    virtual void Commit() override;
    virtual void Rollback() override;

    virtual void Rollback(Savepoint& savepoint) override;
    virtual void ReleaseSavepoint(Savepoint& savepoint) override;
    virtual Savepoint SetSavepoint(const std::string& name) override;

    void Error();

   private:
    void EnsureAutoCommit();

   private:
    SqliteDB db_;              ///< The database connection
    bool auto_commit_ = true;  ///< The auto commit flag
};

}  // namespace sql
#endif
