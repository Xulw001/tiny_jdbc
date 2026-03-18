#include "connection.h"

#include "prepared_statment.h"
#include "savepoint.h"
#include "statement.h"

namespace sql {

SqliteConnection::SqliteConnection(const char* path, const char* params)
    : db_(SqliteDB::Open(path)) {
    if (params) {
        db_.Execute(params);
    }
}

Statement SqliteConnection::CreateStatement() {
    return Statement(new SqliteStatement(*this));
}

PreparedStatement SqliteConnection::PrepareStatement(const std::string& sql) {
    return std::unique_ptr<SqlitePreparedStatement>(
        new SqlitePreparedStatement(*this, sql));
}

void SqliteConnection::set_auto_commit(bool auto_commit) {
    if (auto_commit_ == auto_commit) return;

    auto_commit_ = auto_commit;
    if (auto_commit_) {  // enable autocommit
        db_.Execute("commit;");
    } else {
        db_.Execute("begin;");
    }
}

void SqliteConnection::Commit() {
    if (auto_commit_) throw SQLException("database in auto-commit mode");
    db_.Execute("commit;begin;");
}

void SqliteConnection::Rollback() {
    if (auto_commit_) throw SQLException("database in auto-commit mode");
    db_.Execute("rollback;begin;");
}

void SqliteConnection::Rollback(Savepoint& savepoint) {
    if (auto_commit_) throw SQLException("database in auto-commit mode");
    const std::string sql = "rollback to savepoint " + savepoint->name() + ";";
    db_.Execute(sql.c_str());
}

void SqliteConnection::ReleaseSavepoint(Savepoint& savepoint) {
    if (auto_commit_) throw SQLException("database in auto-commit mode");
    const std::string sql = "release savepoint " + savepoint->name() + ";";
    db_.Execute(sql.c_str());
}

Savepoint SqliteConnection::SetSavepoint(const std::string& name) {
    if (auto_commit_) throw SQLException("database in auto-commit mode");
    const std::string sql = "savepoint " + name + ";";
    db_.Execute(sql.c_str());
    return Savepoint(new SqliteSavepoint(name));
}

void SqliteConnection::EnsureAutoCommit() {
    if (auto_commit_) db_.Execute("begin;commit;");
}

void SqliteConnection::Error() { db_.Error(); }

}  // namespace sql