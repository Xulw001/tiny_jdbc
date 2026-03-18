#include "db.h"

#include "sql/exception.h"

namespace sql {

SqliteDB SqliteDB::Open(const char* filename) {
    sqlite3* raw_db = nullptr;
    if (SQLITE_OK != sqlite3_open_v2(filename, &raw_db,
                                     SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                     NULL)) {
        throw SQLException("open sqlite database failed!");
    }
    return SqliteDB(raw_db);
}

SqliteStmt SqliteDB::Prepare(const char* sql) {
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw SQLException(sqlite3_errmsg(db_.get()));
    return SqliteStmt(stmt);
}

void SqliteDB::Execute(const char* sql) {
    if (sqlite3_exec(db_.get(), sql, nullptr, nullptr, nullptr) == SQLITE_OK)
        return;
    Error();
}

void SqliteDB::Error() {
    if (sqlite3_errcode(db_.get())) {
        throw SQLException(sqlite3_errmsg(db_.get()));
    }
}

}  // namespace sql