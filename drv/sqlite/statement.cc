#include "statement.h"

#include "resultset.h"
#include "sql/exception.h"

namespace sql {

/**
 * @brief Executes a query and returns a result set.
 *
 * @param sql The SQL query to be executed.
 * @return A pointer to the result set.
 */
ResultSet& SqliteStatement::ExecuteQuery(const std::string& sql) {
    stmt_ = conn_.db_.Prepare(sql.c_str());
    result_set_.reset(new SqliteResultSet(conn_, stmt_));
    return result_set_;
}

/**
 * @brief Executes an update statement.
 *
 * This method executes the SQL statement in the `sql` parameter and
 * resets the database connection.
 *
 * @param sql The SQL update statement to be executed.
 */
void SqliteStatement::ExecuteUpdate(const std::string& sql) {
    conn_.db_.Execute(sql.c_str());
    conn_.Reset();
}

/**
 * @brief Adds a SQL statement to the batch.
 *
 * This method adds a SQL statement to the batch of statements to be executed.
 *
 * @param sql The SQL statement to be added to the batch.
 */
void SqliteStatement::AddBatch(const std::string& sql) {
    batch_sqls_.emplace_back(sql);
}

/**
 * @brief Executes the batch of SQL statements.
 *
 * This method executes the batch of SQL statements added through the
 * AddBatch method and resets the database connection.
 *
 * @see AddBatch
 */
void SqliteStatement::ExecuteBatch() {
    for (auto& sql : batch_sqls_) {
        conn_.db_.Execute(sql.c_str());
    }
    conn_.Reset();
}

/**
 * @brief Clears the batch of SQL statements.
 *
 * This method clears the batch of SQL statements added through the
 * AddBatch method.
 *
 * @see AddBatch
 */
void SqliteStatement::ClearBatch() { batch_sqls_.clear(); }

}  // namespace sql