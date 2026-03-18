#include "statement.h"

#include "resultset.h"
#include "sql/exception.h"

namespace sql {

ResultSet& SqliteStatement::ExecuteQuery(const std::string& sql) {
    stmt_ = conn_.db_.Prepare(sql.c_str());
    result_set_.reset(new SqliteResultSet(conn_, stmt_));
    return result_set_;
}

void SqliteStatement::ExecuteUpdate(const std::string& sql) {
    conn_.db_.Execute(sql.c_str());
    conn_.EnsureAutoCommit();
}

void SqliteStatement::AddBatch(const std::string& sql) {
    batch_sqls_.emplace_back(sql);
}

void SqliteStatement::ExecuteBatch() {
    for (auto& sql : batch_sqls_) {
        conn_.db_.Execute(sql.c_str());
    }
    conn_.EnsureAutoCommit();
}

void SqliteStatement::ClearBatch() { batch_sqls_.clear(); }

}  // namespace sql