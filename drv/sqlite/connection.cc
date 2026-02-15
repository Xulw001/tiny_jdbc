#include "connection.h"

#include "prepare_statment.h"
#include "savepoint.h"
#include "statement.h"

namespace sql {

/**
 * @brief Constructor that takes a sqlite3 database connection.
 *
 * This constructor will open the database file and execute the given
 * parameters if set. It will also begin a transaction.
 *
 * @param path The path to the database file.
 * @param params Optional parameters to set on the database connection.
 * @return A SqliteConnection object representing the open database.
 */
SqliteConnection::SqliteConnection(const char* path, const char* params)
    : db_(SqliteDB::Open(path)) {
    if (params) {
        db_.Execute(params);
    }
    BeginTransaction();
}

/**
 * @brief Creates a new statement object.
 *
 * This method creates a new statement object that can be used to execute
 * queries.
 *
 * @return A Statement object representing the new statement.
 */
Statement SqliteConnection::CreateStatement() {
    return Statement(new SqliteStatement(*this));
}

/**
 * @brief Prepares a SQL statement.
 *
 * This method prepares a SQL statement for execution by binding parameters
 * to it.
 *
 * @param sql The SQL statement to be prepared.
 * @return A unique pointer to the prepared statement object.
 */
PreparedStatement SqliteConnection::PrepareStatement(const std::string& sql) {
    return std::unique_ptr<SqlitePreparedStatement>(
        new SqlitePreparedStatement(*this, sql));
}

/**
 * @brief Sets the auto commit status of the connection.
 *
 * If the auto commit status is changed, this method will also reset the
 * connection to its original state.
 *
 * @param auto_commit The auto commit status to be set.
 */
void SqliteConnection::set_auto_commit(bool auto_commit) {
    if (auto_commit_ == auto_commit) return;

    auto_commit_ = auto_commit;
    if (auto_commit_) {  // enable autocommit
        Reset();
    }
}

/**
 * @brief Commit the current transaction.
 *
 * This method commits the current transaction, making all changes
 * permanent.
 *
 * @throws SQLException if the database is in auto-commit mode.
 */
void SqliteConnection::Commit() {
    if (auto_commit_) throw SQLException("database in auto-commit mode");
    db_.Execute("commit;");
}

/**
 * @brief Rollback the current transaction.
 *
 * This method rolls back the current transaction, reverting all changes made.
 *
 * @throws SQLException if the database is in auto-commit mode.
 */
void SqliteConnection::Rollback() {
    if (auto_commit_) throw SQLException("database in auto-commit mode");
    db_.Execute("rollback;");
}

/**
 * @brief Rollback the current transaction to the specified savepoint.
 *
 * This method rolls back the current transaction to the specified savepoint,
 * reverting all changes made since the savepoint was created.
 *
 * @param savepoint The savepoint to rollback to.
 * @throws SQLException if the database is in auto-commit mode.
 */
void SqliteConnection::Rollback(Savepoint savepoint) {
    if (auto_commit_) throw SQLException("database in auto-commit mode");
    const std::string sql = "rollback to savepoint " + savepoint->name() + ";";
    db_.Execute(sql.c_str());
}

/**
 * @brief Release a savepoint.
 *
 * This method releases a savepoint, allowing the database to
 * reclaim any resources associated with it.
 *
 * @param savepoint The savepoint to release.
 * @throws SQLException if the database is in auto-commit mode.
 */
void SqliteConnection::ReleaseSavepoint(Savepoint savepoint) {
    if (auto_commit_) throw SQLException("database in auto-commit mode");
    const std::string sql = "release savepoint " + savepoint->name() + ";";
    db_.Execute(sql.c_str());
}

/**
 * @brief Create a savepoint.
 *
 * This method creates a savepoint, which can be used to rollback the current
 * transaction to a specific point in time.
 *
 * @param name The name of the savepoint to create.
 * @return A savepoint object that can be used to rollback the current
 * transaction.
 * @throws SQLException if the database is in auto-commit mode.
 */
Savepoint SqliteConnection::SetSavepoint(const std::string& name) {
    if (auto_commit_) throw SQLException("database in auto-commit mode");
    const std::string sql = "savepoint " + name + ";";
    db_.Execute(sql.c_str());
    return Savepoint(new SqliteSavepoint(name));
}

/**
 * @brief Begin a new transaction.
 *
 * This method starts a new transaction by executing the "begin" statement.
 */
void SqliteConnection::BeginTransaction() { db_.Execute("begin;"); }

/**
 * @brief Reset the connection to its original state.
 *
 * If the auto commit status is true, this method will commit the current
 * transaction and start a new one. Otherwise, it does nothing.
 */
void SqliteConnection::Reset() {
    if (auto_commit_) {
        db_.Execute("commit;begin;");
    }
}

void SqliteConnection::Error() { db_.Error(); }

}  // namespace sql