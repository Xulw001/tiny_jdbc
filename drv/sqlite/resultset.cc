#include "resultset.h"

#include <algorithm>

#include "sql/exception.h"

namespace sql {

/**
 * @brief Moves the result set to the next row.
 *
 * This method moves the result set to the next row and returns true if
 * there are more rows to retrieve. If there are no more rows to retrieve,
 * it returns false. If an error occurs while retrieving the next row,
 * it throws an exception.
 *
 * @return True if there are more rows to retrieve, false otherwise.
 */
bool SqliteResultSet::NextRow() {
    auto rc = stmt_.Step();
    switch (rc) {
    case SQLITE_DONE:
        return false;
    case SQLITE_ROW:
        return true;
    default:
        conn_.Error();
    }
    return false;
}

/**
 * @brief Retrieves a string value from the current row.
 *
 * This method retrieves a string value from the current row at the
 * specified index and returns it.
 *
 * @param index The index of the column to be retrieved.
 * @return The value at the specified index as a string.
 */
Text SqliteResultSet::GetString(int index) {
    Text value("");
    value_ = stmt_.Value(index - 1, value, ValueType::TEXT);
    return std::move(value);
}

/**
 * @brief Retrieves a string value from the current row by column name.
 *
 * This method retrieves a string value from the current row by the
 * specified column name and returns it.
 *
 * @param column_name The name of the column to be retrieved.
 * @return The value of the specified column as a string.
 */
Text SqliteResultSet::GetString(const std::string& column_name) {
    return std::move(GetString(stmt_.GetIndexByName(column_name) + 1));
}

/**
 * @brief Retrieves an integer value from the current row.
 *
 * This method retrieves an integer value from the current row at the
 * specified index and returns it.
 *
 * @param index The index of the column to be retrieved.
 * @return The value at the specified index as an integer.
 */
Integer SqliteResultSet::GetInteger(int index) {
    Integer value(0);
    value_ = stmt_.Value(index - 1, value, ValueType::INTEGER);
    return std::move(value);
}

/**
 * @brief Retrieves an integer value from the current row by column name.
 *
 * This method retrieves an integer value from the current row by the
 * specified column name and returns it.
 *
 * @param column_name The name of the column to be retrieved.
 * @return The value of the specified column as an integer.
 */
Integer SqliteResultSet::GetInteger(const std::string& column_name) {
    return std::move(GetInteger(stmt_.GetIndexByName(column_name) + 1));
}

/**
 * @brief Retrieves a decimal value from the current row.
 *
 * This method retrieves a decimal value from the current row at the
 * specified index and returns it.
 *
 * @param index The index of the column to be retrieved.
 * @return The value at the specified index as a decimal.
 */
Decimal SqliteResultSet::GetDecimal(int index) {
    Decimal value(0.0);
    value_ = stmt_.Value(index - 1, value, ValueType::REAL);
    return std::move(value);
}

/**
 * @brief Retrieves a decimal value from the current row by column name.
 *
 * This method retrieves a decimal value from the current row by the
 * specified column name and returns it.
 *
 * @param column_name The name of the column to be retrieved.
 * @return The value of the specified column as a decimal.
 */
Decimal SqliteResultSet::GetDecimal(const std::string& column_name) {
    return std::move(GetDecimal(stmt_.GetIndexByName(column_name) + 1));
}

/**
 * @brief Retrieves a boolean value from the current row.
 *
 * This method retrieves a boolean value from the current row at the
 * specified index and returns it.
 *
 * @param index The index of the column to be retrieved.
 * @return The value at the specified index as a boolean.
 */
Boolean SqliteResultSet::GetBool(int index) {
    Boolean value(0);
    value_ = stmt_.Value(index - 1, value, ValueType::INTEGER);
    return std::move(value);
}

/**
 * @brief Retrieves a boolean value from the current row by column name.
 *
 * This method retrieves a boolean value from the current row by the
 * specified column name and returns it.
 *
 * @param column_name The name of the column to be retrieved.
 * @return The value of the specified column as a boolean.
 */
Boolean SqliteResultSet::GetBool(const std::string& column_name) {
    return std::move(GetBool(stmt_.GetIndexByName(column_name) + 1));
}

/**
 * @brief Retrieves a blob value from the current row.
 *
 * This method retrieves a blob value from the current row at the
 * specified index and returns it.
 *
 * @param index The index of the column to be retrieved.
 * @return The value at the specified index as a blob.
 */
Blob SqliteResultSet::GetBlob(int index) {
    Blob value("", 0);
    value_ = stmt_.Value(index - 1, value, ValueType::BLOB);
    return std::move(value);
}

/**
 * @brief Retrieves a blob value from the current row by column name.
 *
 * This method retrieves a blob value from the current row by the
 * specified column name and returns it.
 *
 * @param column_name The name of the column to be retrieved.
 * @return The value of the specified column as a blob.
 */
Blob SqliteResultSet::GetBlob(const std::string& column_name) {
    return std::move(GetBlob(stmt_.GetIndexByName(column_name) + 1));
}
};  // namespace sql