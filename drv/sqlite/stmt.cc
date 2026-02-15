#include "stmt.h"

#include "sql/exception.h"

namespace sql {

/**
 * @brief Step the prepared statement.
 *
 * This function steps the prepared statement and resets it if it reaches
 * the end of the result set.
 *
 * @return The result of sqlite3_step() or SQLITE_DONE if the end of
 *         the result set is reached.
 */
int SqliteStmt::Step() {
    int ret = sqlite3_step(stmt_.get());
    if (ret == SQLITE_DONE) {
        sqlite3_reset(stmt_.get());
        sqlite3_clear_bindings(stmt_.get());
    }
    return ret;
}

/**
 * @brief Update the column count and map
 *
 * This function updates the column count and constructs a map
 * from column name to column index.
 *
 * @note This function should be called after Step() returns SQLITE_ROW
 */
void SqliteStmt::UpdateColumn() {
    col_count_ = sqlite3_column_count(stmt_.get());
    for (int i = 0; i < col_count_; i++) {
        col_map_.emplace(sqlite3_column_name(stmt_.get(), i), i);
    };
}

/**
 * @brief Get the column index by column name
 * @param col_name the column name
 * @return the column index
 * @throws SQLException if the column name is unknown
 */
int SqliteStmt::GetIndexByName(const std::string& col_name) {
    auto it = col_map_.find(col_name);
    if (it == col_map_.end()) throw SQLException("unknown column name!");
    return it->second;
}

/**
 * @brief Get the number of parameters in the prepared statement
 *
 * This function returns the number of parameters in the prepared statement.
 *
 * @return the number of parameters
 */
int SqliteStmt::column_bind_count() {
    return sqlite3_bind_parameter_count(stmt_.get());
}

/**
 * @brief Bind a value to a parameter in the prepared statement
 *
 * This function binds a value to a parameter in the prepared statement.
 * The type of the value to be bound is specified by the type parameter.
 * The value to be bound is specified by the obj parameter.
 * The index of the parameter to be bound is specified by the index parameter.
 *
 * @param index the index of the parameter
 * @param obj the value to be bound
 * @param type the type of the value to be bound
 * @return true if the bind is successful, false otherwise
 */
bool SqliteStmt::Bind(int index, reflect::ObjectInternal& obj, ValueType type) {
    switch (type) {
    case ValueType::BLOB: {
        auto& blob = static_cast<Blob&>(obj);
        if (SQLITE_OK != sqlite3_bind_blob(stmt_.get(), index, blob.ptr(),
                                           blob.size(), SQLITE_TRANSIENT)) {
            return false;
        }
    } break;
    case ValueType::INTEGER: {
        auto& num = static_cast<Integer&>(obj);
        if (SQLITE_OK !=
            sqlite3_bind_int64(stmt_.get(), index, num.to_int64())) {
            return false;
        }
    } break;
    case ValueType::NULLVAL: {
        if (SQLITE_OK != sqlite3_bind_null(stmt_.get(), index)) {
            return false;
        }
    } break;
    case ValueType::REAL: {
        auto& dec = static_cast<Decimal&>(obj);
        if (SQLITE_OK !=
            sqlite3_bind_double(stmt_.get(), index, dec.to_double())) {
            return false;
        }
    } break;
    case ValueType::TEXT: {
        auto& str = static_cast<Text&>(obj);
        if (SQLITE_OK != sqlite3_bind_text(stmt_.get(), index,
                                           str.get().c_str(), -1,
                                           SQLITE_TRANSIENT)) {
            return false;
        }
    } break;
    }
    return true;
}

/**
 * @brief Extract a value from the statement.
 *
 * This function extracts a value from the statement and stores it in obj.
 * The type of the value is checked against type, and if the types do not match,
 * false is returned. The value is extracted based on the type of the column.
 *
 * @param index The index of the column.
 * @param obj The object to store the value.
 * @param type The type of the value.
 * @return True if the value is successfully extracted, false otherwise.
 */
bool SqliteStmt::Value(int index, reflect::ObjectInternal& obj,
                       ValueType type) {
    switch (sqlite3_column_type(stmt_.get(), index)) {
    case SQLITE_INTEGER:
        TypeCheck(SQLITE_INTEGER, type);
        static_cast<Integer&>(obj) =
            Integer(sqlite3_column_int64(stmt_.get(), index));
        break;
    case SQLITE_FLOAT:
        TypeCheck(SQLITE_FLOAT, type);
        static_cast<Decimal&>(obj) =
            Decimal(sqlite3_column_double(stmt_.get(), index));
        break;
    case SQLITE_BLOB:
        TypeCheck(SQLITE_BLOB, type);
        static_cast<Blob&>(obj) =
            Blob(sqlite3_column_blob(stmt_.get(), index),
                 sqlite3_column_bytes(stmt_.get(), index));
        break;
    case SQLITE_TEXT:
        TypeCheck(SQLITE_TEXT, type);
        static_cast<Text&>(obj) =
            Text((char*)sqlite3_column_text(stmt_.get(), index));
        break;
    case SQLITE_NULL:
    default:
        return false;
        break;
    }
    return true;
}

/**
 * @brief Check if the column type matches the given type.
 *
 * This function checks if the given column type matches the given
 * type. If the types do not match, an exception is thrown.
 *
 * @param col_type The type of the column.
 * @param type The type to check against.
 * @throws SQLException If the types do not match.
 */
void SqliteStmt::TypeCheck(int col_type, ValueType type) {
    if (col_type != static_cast<int>(type))
        throw SQLException("invalid data type!");
}

}  // namespace sql