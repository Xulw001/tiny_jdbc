/**
 * @file resultset.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the ResultSetBase class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef SQL_RESULTSET_H
#define SQL_RESULTSET_H

#include <memory>
#include <string>

#include "sql/value.h"

namespace sql {

/**
 * @brief A base class for result sets returned from SQL queries.
 *
 * This class provides an interface for retrieving values from a result set.
 * It is designed to be used by a derived class that provides a specific
 * implementation for a particular database management system.
 *
 * @note The class is pure virtual and must be implemented by a derived class.
 */
class ResultSetBase {
   public:
    virtual ~ResultSetBase() = default;
    /**
     * @brief Advances the result set to the next row.
     *
     * This method advances the result set to the next row and returns true
     * if there are more rows to be retrieved.
     *
     * @return true if there are more rows, false otherwise
     */
    virtual bool NextRow() = 0;
    /**
     * @brief Retrieves a string value from the current row.
     *
     * This method retrieves a string value from the current row at the
     * specified index and returns it.
     *
     * @param index The index of the column to be retrieved.
     * @return The value at the specified index as a string.
     */
    virtual Text GetString(int index) = 0;
    /**
     * @brief Retrieves a string value from the current row by column name.
     *
     * This method retrieves a string value from the current row by the
     * specified column name and returns it.
     *
     * @param column_name The name of the column to be retrieved.
     * @return The value of the specified column as a string.
     */
    virtual Text GetString(const std::string& column_name) = 0;
    /**
     * @brief Retrieves an integer value from the current row.
     *
     * This method retrieves an integer value from the current row at the
     * specified index and returns it.
     *
     * @param index The index of the column to be retrieved.
     * @return The value at the specified index as an integer.
     */
    virtual Integer GetInteger(int index) = 0;
    /**
     * @brief Retrieves an integer value from the current row by column name.
     *
     * This method retrieves an integer value from the current row by the
     * specified column name and returns it.
     *
     * @param column_name The name of the column to be retrieved.
     * @return The value of the specified column as an integer.
     */
    virtual Integer GetInteger(const std::string& column_name) = 0;
    /**
     * @brief Retrieves a decimal value from the current row.
     *
     * This method retrieves a decimal value from the current row at the
     * specified index and returns it.
     *
     * @param index The index of the column to be retrieved.
     * @return The value at the specified index as a decimal.
     */
    virtual Decimal GetDecimal(int index) = 0;
    /**
     * @brief Retrieves a decimal value from the current row by column name.
     *
     * This method retrieves a decimal value from the current row by the
     * specified column name and returns it.
     *
     * @param column_name The name of the column to be retrieved.
     * @return The value of the specified column as a decimal.
     */
    virtual Decimal GetDecimal(const std::string& column_name) = 0;
    /**
     * @brief Retrieves a boolean value from the current row.
     *
     * This method retrieves a boolean value from the current row at the
     * specified index and returns it.
     *
     * @param index The index of the column to be retrieved.
     * @return The value at the specified index as a boolean.
     */
    virtual Boolean GetBool(int index) = 0;
    /**
     * @brief Retrieves a boolean value from the current row by column name.
     *
     * This method retrieves a boolean value from the current row by the
     * specified column name and returns it.
     *
     * @param column_name The name of the column to be retrieved.
     * @return The value of the specified column as a boolean.
     */
    virtual Boolean GetBool(const std::string& column_name) = 0;
    /**
     * @brief Retrieves a blob value from the current row.
     *
     * This method retrieves a blob value from the current row at the specified
     * index and returns it.
     *
     * @param index The index of the column to be retrieved.
     * @return The value at the specified index as a blob.
     */
    virtual Blob GetBlob(int index) = 0;
    /**
     * @brief Retrieves a blob value from the current row by column name.
     *
     * This method retrieves a blob value from the current row by the specified
     * column name and returns it.
     *
     * @param column_name The name of the column to be retrieved.
     * @return The value of the specified column as a blob.
     */
    virtual Blob GetBlob(const std::string& column_name) = 0;
    /**
     * @brief Checks if the current row is null.
     *
     * This method checks if the current row is null and returns true
     * if it is.
     *
     * @return true if the current row is null, false otherwise
     */
    virtual bool IsNull() = 0;
};

using ResultSet = std::unique_ptr<ResultSetBase>;
}  // namespace sql
#endif