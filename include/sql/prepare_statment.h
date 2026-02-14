/**
 * @file prepare_statment.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the PreparedStatementBase class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef SQL_PREPEARE_STATMENT_H
#define SQL_PREPEARE_STATMENT_H

#include "exception.h"
#include "statement.h"

namespace sql {
/**
 * @brief Abstract base class for prepared statements.
 *
 * This class provides a pure virtual interface for prepared statements.
 * It is designed to be used by a derived class that provides a specific
 * implementation for a particular database management system.
 */
class PreparedStatementBase : public StatementBase {
   public:
    virtual ~PreparedStatementBase() = default;
    /**
     * @brief Execute a query using this prepared statement.
     *
     * This function executes a query using this prepared statement and
     * returns the result set.
     *
     * @return A result set object containing the results of the query.
     */
    virtual ResultSet& ExecuteQuery() = 0;

    /**
     * @brief Execute an update query using this prepared statement.
     *
     * This function executes an update query using this prepared statement
     * without returning any result set.
     */
    virtual void ExecuteUpdate() = 0;

    /**
     * @brief Set a string parameter in this prepared statement.
     *
     * This function sets a string parameter in this prepared statement.
     *
     * @param index The index of the parameter to be set.
     * @param value The value to be set.
     */
    virtual void SetString(int index, const Text& value) = 0;

    /**
     * @brief Set an integer parameter in this prepared statement.
     *
     * This function sets an integer parameter in this prepared statement.
     *
     * @param index The index of the parameter to be set.
     * @param value The value to be set.
     */
    virtual void SetInteger(int index, const Integer& value) = 0;

    /**
     * @brief Set a decimal parameter in this prepared statement.
     *
     * This function sets a decimal parameter in this prepared statement.
     *
     * @param index The index of the parameter to be set.
     * @param value The value to be set.
     */
    virtual void SetDecimal(int index, const Decimal& value) = 0;

    /**
     * @brief Set a boolean parameter in this prepared statement.
     *
     * This function sets a boolean parameter in this prepared statement.
     *
     * @param index The index of the parameter to be set.
     * @param value The value to be set.
     */
    virtual void SetBool(int index, const Boolean& value) = 0;

    /**
     * @brief Set a null parameter in this prepared statement.
     *
     * This function sets a null parameter in this prepared statement.
     *
     * @param index The index of the parameter to be set.
     */
    virtual void SetNull(int index) = 0;

    /**
     * @brief Set a blob parameter in this prepared statement.
     *
     * This function sets a blob parameter in this prepared statement.
     *
     * @param index The index of the parameter to be set.
     * @param value The value to be set.
     */
    virtual void SetBlob(int index, const Blob& value) = 0;

    /**
     * @brief Add a batch of queries to this prepared statement.
     *
     * This function adds a batch of queries to this prepared statement.
     */
    virtual void AddBatch() = 0;

    /**
     * @brief Clear the parameters of this prepared statement.
     *
     * This function clears the parameters of this prepared statement.
     */
    virtual void ClearParameters() = 0;

    /**
     * @brief Execute the batch of queries in this prepared statement.
     *
     * This function executes the batch of queries in this prepared statement.
     */
    virtual void ExecuteBatch() = 0;

    virtual ResultSet& ExecuteQuery(const std::string& sql) override {
        throw SQLException("unsupported call!");
    }

    virtual void ExecuteUpdate(const std::string& sql) override {
        throw SQLException("unsupported call!");
    }

    virtual void AddBatch(const std::string& sql) override {
        throw SQLException("unsupported call!");
    }
};

using PreparedStatement = std::unique_ptr<PreparedStatementBase>;
}  // namespace sql

#endif