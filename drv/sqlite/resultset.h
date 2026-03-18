/**
 * @file resultset.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief SQLite implementation of result set
 * @version 0.2
 * @date 2026-03-14
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
 * @brief SQLite implementation of ResultSetBase
 * @details Implements result set operations for SQLite query results
 */
class SqliteResultSet : public ResultSetBase {
   public:
    /**
     * @brief Construct a new SqliteResultSet object
     * @param conn Reference to SqliteConnection
     * @param stmt Reference to SqliteStmt
     */
    SqliteResultSet(SqliteConnection& conn, SqliteStmt& stmt)
        : conn_(conn), stmt_(stmt) {
        stmt.UpdateColumn();
    }

    virtual ~SqliteResultSet() = default;

    /**
     * @brief Move to the next row in the result set
     * @return True if there is a next row, false otherwise
     */
    virtual bool NextRow() override;

    /**
     * @brief Get string value by column index
     * @param index Column index (1-based)
     * @return String value at the specified column
     */
    virtual std::string GetString(int index) override;

    /**
     * @brief Get string value by column name
     * @param column_name Column name
     * @return String value at the specified column
     */
    virtual std::string GetString(const std::string& column_name) override;

    /**
     * @brief Get boolean value by column index
     * @param index Column index (1-based)
     * @return Boolean value at the specified column
     */
    virtual bool GetBoolean(int index) override;

    /**
     * @brief Get boolean value by column name
     * @param column_name Column name
     * @return Boolean value at the specified column
     */
    virtual bool GetBoolean(const std::string& column_name) override;

    /**
     * @brief Get short value by column index
     * @param index Column index (1-based)
     * @return Short value at the specified column
     */
    virtual short GetShort(int index) override;

    /**
     * @brief Get short value by column name
     * @param column_name Column name
     * @return Short value at the specified column
     */
    virtual short GetShort(const std::string& column_name) override;

    /**
     * @brief Get int value by column index
     * @param index Column index (1-based)
     * @return Int value at the specified column
     */
    virtual int GetInt(int index) override;

    /**
     * @brief Get int value by column name
     * @param column_name Column name
     * @return Int value at the specified column
     */
    virtual int GetInt(const std::string& column_name) override;

    /**
     * @brief Get long long value by column index
     * @param index Column index (1-based)
     * @return Long long value at the specified column
     */
    virtual long long GetLong(int index) override;

    /**
     * @brief Get long long value by column name
     * @param column_name Column name
     * @return Long long value at the specified column
     */
    virtual long long GetLong(const std::string& column_name) override;

    /**
     * @brief Get double value by column index
     * @param index Column index (1-based)
     * @return Double value at the specified column
     */
    virtual double GetDouble(int index) override;

    /**
     * @brief Get double value by column name
     * @param column_name Column name
     * @return Double value at the specified column
     */
    virtual double GetDouble(const std::string& column_name) override;

    /**
     * @brief Get float value by column index
     * @param index Column index (1-based)
     * @return Float value at the specified column
     */
    virtual float GetFloat(int index) override;

    /**
     * @brief Get float value by column name
     * @param column_name Column name
     * @return Float value at the specified column
     */
    virtual float GetFloat(const std::string& column_name) override;

    /**
     * @brief Get blob value by column index
     * @param index Column index (1-based)
     * @return Blob value at the specified column
     */
    virtual Blob GetBlob(int index) override;

    /**
     * @brief Get blob value by column name
     * @param column_name Column name
     * @return Blob value at the specified column
     */
    virtual Blob GetBlob(const std::string& column_name) override;

    /**
     * @brief Check if the current value is NULL
     * @return True if the current value is NULL, false otherwise
     */
    virtual bool IsNull() override { return !has_value_; }

   private:
    SqliteConnection& conn_;  ///< Reference to SQLite connection
    SqliteStmt& stmt_;        ///< Reference to SQLite statement
    bool has_value_ = false;  ///< Flag indicating if the result set has a value
};

}  // namespace sql
#endif