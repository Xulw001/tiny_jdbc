/**
 * @file resultset.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the SqliteResultSet class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef DRV_SQLITE_RESULTSET_H
#define DRV_SQLITE_RESULTSET_H

#include "connection.h"
#include "sql/resultset.h"
#include "stmt.h"

namespace sql {

/**
 * @class SqliteResultSet
 *
 * This class provides a RAII interface to a sqlite3 result set.
 *
 * @brief A class that wraps a sqlite3 result set.
 */
class SqliteResultSet : public ResultSetBase {
   public:
    /**
     * @brief Constructor.
     *
     * The constructor updates the column information of the statement object.
     *
     * @param conn The connection object to the sqlite database.
     * @param stmt The statement object from which the result set is retrieved.
     */
    SqliteResultSet(SqliteConnection& conn, SqliteStmt& stmt)
        : conn_(conn), stmt_(stmt) {
        stmt.UpdateColumn();
    }

    virtual ~SqliteResultSet() = default;
    virtual bool NextRow() override;
    virtual Text GetString(int index) override;
    virtual Text GetString(const std::string& column_name) override;
    virtual Integer GetInteger(int index) override;
    virtual Integer GetInteger(const std::string& column_name) override;
    virtual Decimal GetDecimal(int index) override;
    virtual Decimal GetDecimal(const std::string& column_name) override;
    virtual Boolean GetBool(int index) override;
    virtual Boolean GetBool(const std::string& column_name) override;
    virtual Blob GetBlob(int index) override;
    virtual Blob GetBlob(const std::string& column_name) override;
    virtual bool IsNull() override { return !value_; }

   private:
    SqliteConnection& conn_;  ///< The database connection
    SqliteStmt& stmt_;        ///< The prepared statement
    bool value_ = false;      ///< If has value
};

}  // namespace sql
#endif