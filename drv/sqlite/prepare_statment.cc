#include "prepare_statment.h"

#include "resultset.h"

namespace sql {

/** @brief Executes a query using this prepared statement.
 *
 * This function executes a query using this prepared statement and returns a
 * result set.  The result set contains the results of the query.
 *
 * @return A result set object containing the results of the query.
 */
ResultSet& SqlitePreparedStatement::ExecuteQuery() {
    Bind(batch_params_.back());
    result_set_.reset(new SqliteResultSet(conn_, stmt_));
    return result_set_;
}

/**
 * @brief Executes an update query using this prepared statement.
 *
 * This function executes an update query using this prepared statement
 * without returning any result set.
 *
 * @note This function assumes that the prepared statement has already been
 * bound with parameters using the Set<type> functions.
 */
void SqlitePreparedStatement::ExecuteUpdate() {
    Bind(batch_params_.back());
    if (stmt_.Step() != SQLITE_DONE) {
        conn_.Error();
    }
    conn_.Reset();
    batch_params_.pop_back();
}

/**
 * @brief Set a string parameter in this prepared statement.
 *
 * This function sets a string parameter in this prepared statement.
 *
 * @param index The index of the parameter to be set.
 * @param value The value to be set.
 */
void SqlitePreparedStatement::SetString(int index, const Text& value) {
    SetValue<Text>(index - 1, ValueType::TEXT, value);
}

/**
 * @brief Set an integer parameter in this prepared statement.
 *
 * This function sets an integer parameter in this prepared statement.
 *
 * @param index The index of the parameter to be set.
 * @param value The value to be set.
 */
void SqlitePreparedStatement::SetInteger(int index, const Integer& value) {
    SetValue<Integer>(index - 1, ValueType::INTEGER, value);
}

/**
 * @brief Set a decimal parameter in this prepared statement.
 *
 * This function sets a decimal parameter in this prepared statement.
 *
 * @param index The index of the parameter to be set.
 * @param value The value to be set.
 */
void SqlitePreparedStatement::SetDecimal(int index, const Decimal& value) {
    SetValue<Decimal>(index - 1, ValueType::REAL, value);
}

/**
 * @brief Set a boolean parameter in this prepared statement.
 *
 * This function sets a boolean parameter in this prepared statement.
 *
 * @param index The index of the parameter to be set.
 * @param value The value to be set.
 */
void SqlitePreparedStatement::SetBool(int index, const Boolean& value) {
    SetValue<Boolean>(index - 1, ValueType::INTEGER, value);
}

/**
 * @brief Set a null parameter in this prepared statement.
 */
void SqlitePreparedStatement::SetNull(int index) { ; }

/**
 * @brief Set a blob parameter in this prepared statement.
 *
 * This function sets a blob parameter in this prepared statement.
 *
 * @param index The index of the parameter to be set.
 * @param value The value to be set.
 */
void SqlitePreparedStatement::SetBlob(int index, const Blob& value) {
    SetValue<Blob>(index - 1, ValueType::BLOB, value);
}

/**
 * @brief Add a batch of parameters.
 */
void SqlitePreparedStatement::AddBatch() { batch_params_.emplace_back(); }

/**
 * @brief Clear the parameter list.
 *
 * This function clears the current parameter.
 */
void SqlitePreparedStatement::ClearParameters() { GetParam().clear(); }

/**
 * @brief Clear the batch of parameters.
 *
 * This function clears the batch of parameters added through the
 * AddBatch method.
 *
 * @see AddBatch
 */
void SqlitePreparedStatement::ClearBatch() { batch_params_.clear(); }

/**
 * @brief Execute a batch of SQL statements.
 *
 * This function executes a batch of SQL statements added through the
 * AddBatch method and resets the database connection.
 *
 * @see AddBatch
 */
void SqlitePreparedStatement::ExecuteBatch() {
    while (batch_params_.size() > 1) {
        auto& param = batch_params_.front();
        Bind(batch_params_.front());
        if (stmt_.Step() != SQLITE_DONE) {
            conn_.Error();
        }
        batch_params_.pop_front();
    }
    conn_.Reset();
}

/**
 * @brief Get the current parameter list.
 *
 * This function returns a reference to the current parameter list.
 *
 * @return A reference to the current parameter list.
 */
Value& SqlitePreparedStatement::GetParam() {
    auto& param = batch_params_.back();
    if (param.empty()) {
        for (int i = 0; i < stmt_.column_bind_count(); i++) {
            param.emplace_back(ValueType::NULLVAL, nullptr);
        }
    }
    return param;
}

/**
 * @brief Set a parameter value.
 *
 * This function sets a parameter value in the parameter list. The parameter
 * list is maintained internally by the prepared statement. The index
 * is 0-based and corresponds to the position of the parameter
 * in the SQL statement.
 *
 * @param index The index of the parameter to be set.
 * @param type The type of the parameter value.
 * @param value The value of the parameter.
 */
template <typename T>
void SqlitePreparedStatement::SetValue(int index, ValueType type,
                                       const T& value) {
    auto& param = GetParam();
    param[index] = std::move(std::make_pair(type, Pointer(new T(value))));
}

/**
 * @brief Bind a parameter value.
 *
 * This function binds a parameter value to the prepared statement. It checks
 * if the number of parameters in the value matches the number of
 * columns in the prepared statement. If not, it throws an exception.
 * Then it iterates over the parameters in the value and binds them to the
 * prepared statement using the Bind() function of the SqliteStmt class.
 *
 * @param value A value object containing the parameter values to be bound.
 */
void SqlitePreparedStatement::Bind(Value& value) {
    if (stmt_.column_bind_count() < value.size())
        throw SQLException("param count miss!");

    for (int i = 0; i < value.size(); i++) {
        if (!stmt_.Bind(i + 1, value[i].second, value[i].first)) {
            conn_.Error();
        }
    };
}

}  // namespace sql