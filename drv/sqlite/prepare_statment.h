/**
 * @file prepare_statment.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the SqlitePreparedStatement class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef DRV_SQLITE_PREPARE_STATMENT_H
#define DRV_SQLITE_PREPARE_STATMENT_H

#include <list>

#include "connection.h"
#include "sql/prepare_statment.h"
#include "stmt.h"

namespace sql {

using reflect::Pointer;

/**
 * @class SqlitePreparedStatement
 *
 * A class that provides a RAII interface to a sqlite3 prepared
 * statement.
 *
 * @brief A class that wraps a sqlite3 prepared statement.
 */
class SqlitePreparedStatement : public PreparedStatementBase {
   public:
    /**
     * @brief Constructor.
     *
     * This constructor creates a new prepared statement object using the given
     * connection object and SQL query.
     *
     * @param conn The connection object to the sqlite database.
     * @param sql The SQL query to be prepared.
     */
    explicit SqlitePreparedStatement(SqliteConnection& conn,
                                     const std::string& sql)
        : conn_(conn), stmt_(conn_.db_.Prepare(sql.c_str())) {
        batch_params_.emplace_back();
    }

    virtual ~SqlitePreparedStatement() = default;
    virtual ResultSet& ExecuteQuery() override;
    virtual void ExecuteUpdate() override;
    virtual ResultSet& GetResultSet() override { return result_set_; }
    virtual void SetString(int index, const Text& value) override;
    virtual void SetInteger(int index, const Integer& value) override;
    virtual void SetDecimal(int index, const Decimal& value) override;
    virtual void SetBool(int index, const Boolean& value) override;
    virtual void SetNull(int index) override;
    virtual void SetBlob(int index, const Blob& value) override;
    virtual void AddBatch() override;
    virtual void ClearParameters() override;
    virtual void ExecuteBatch() override;
    virtual void ClearBatch() override;

   private:
    Value& GetParam();

    template <typename T>
    void SetValue(int index, ValueType type, const T& value);

    void Bind(Value& value);

   private:
    SqliteConnection& conn_;
    SqliteStmt stmt_;
    std::list<Value> batch_params_;
    ResultSet result_set_ = nullptr;
};

}  // namespace sql

#endif