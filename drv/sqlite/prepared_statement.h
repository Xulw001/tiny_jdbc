/**
 * @file prepared_statment.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief SQLite implementation of prepared statement
 * @version 0.2
 * @date 2026-03-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef DRV_SQLITE_PREPARED_STATMENT_H
#define DRV_SQLITE_PREPARED_STATMENT_H

#include <list>

#include "connection.h"
#include "sql/prepared_statement.h"
#include "stmt.h"

namespace sql {
/**
 * @class SqlitePreparedStatement
 * @brief SQLite implementation of PreparedStatementBase
 * @details Implements prepared statement operations for SQLite
 */
class SqlitePreparedStatement : public PreparedStatementBase {
   public:
    /**
     * @brief Construct a new SqlitePreparedStatement object
     * @param conn Reference to SqliteConnection
     * @param sql SQL statement string
     */
    explicit SqlitePreparedStatement(SqliteConnection& conn,
                                     const std::string& sql)
        : conn_(conn), stmt_(conn_.db_.Prepare(sql.c_str())) {
        batch_params_.emplace_back();
    }

    virtual ~SqlitePreparedStatement() = default;

    /**
     * @brief Execute the prepared query and return a ResultSet
     * @return Result set containing query results
     */
    virtual ResultSet ExecuteQuery() override;

    /**
     * @brief Execute the prepared update statement
     */
    virtual void ExecuteUpdate() override;

    /**
     * @brief Get the current result set
     * @return Current result set
     */
    virtual ResultSet GetResultSet() override { return result_set_; }

    /**
     * @brief Set string parameter
     * @param index Parameter index (1-based)
     * @param value String value to set
     */
    virtual void SetString(int index, std::string value) override;

    /**
     * @brief Set float parameter
     * @param index Parameter index (1-based)
     * @param value Float value to set
     */
    virtual void SetFloat(int index, float value) override;

    /**
     * @brief Set double parameter
     * @param index Parameter index (1-based)
     * @param value Double value to set
     */
    virtual void SetDouble(int index, double value) override;

    /**
     * @brief Set short parameter
     * @param index Parameter index (1-based)
     * @param value Short value to set
     */
    virtual void SetShort(int index, short value) override;

    /**
     * @brief Set int parameter
     * @param index Parameter index (1-based)
     * @param value Int value to set
     */
    virtual void SetInt(int index, int value) override;

    /**
     * @brief Set long long parameter
     * @param index Parameter index (1-based)
     * @param value Long long value to set
     */
    virtual void SetLong(int index, long long value) override;

    /**
     * @brief Set boolean parameter
     * @param index Parameter index (1-based)
     * @param value Boolean value to set
     */
    virtual void SetBoolean(int index, bool value) override;

    /**
     * @brief Set NULL parameter
     * @param index Parameter index (1-based)
     */
    virtual void SetNull(int index) override;

    /**
     * @brief Set blob parameter
     * @param index Parameter index (1-based)
     * @param value Blob value to set
     */
    virtual void SetBlob(int index, Blob value) override;

    /**
     * @brief Add the current parameter set to the batch
     */
    virtual void AddBatch() override;

    /**
     * @brief Clear all parameters
     */
    virtual void ClearParameters() override;

    /**
     * @brief Execute all parameter sets in the batch
     */
    virtual void ExecuteBatch() override;

    /**
     * @brief Clear all batch parameters
     */
    virtual void ClearBatch() override;

   private:
    /**
     * @brief Get current batch parameters
     * @return Iterator to current batch parameters
     */
    std::list<std::vector<Value>>::iterator GetParam();

    /**
     * @brief Set value at specified index
     * @param index Parameter index (1-based)
     * @param value Value to set
     */
    void SetValue(int index, Value&& value);

    /**
     * @brief Bind parameters to statement
     * @param itr Iterator to batch parameters
     */
    void Bind(std::list<std::vector<Value>>::iterator itr);

   private:
    SqliteConnection& conn_;  ///< Reference to SQLite connection
    SqliteStmt stmt_;         ///< SQLite statement wrapper
    std::list<std::vector<Value>> batch_params_;  ///< Batch parameters
    std::unique_ptr<sql::ResultSetBase> result_set_ =
        nullptr;  ///< Current result set
};

}  // namespace sql

#endif