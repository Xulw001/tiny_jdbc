#include "prepared_statement.h"

#include "resultset.h"

namespace sql {

ResultSet SqlitePreparedStatement::ExecuteQuery() {
    Bind(batch_params_.begin());
    result_set_.reset(new SqliteResultSet(conn_, stmt_));
    return result_set_;
}

void SqlitePreparedStatement::ExecuteUpdate() {
    do {
        Bind(batch_params_.begin());
        if (stmt_.Step() != SQLITE_DONE) {
            break;
        }
    } while (false);
    conn_.EnsureAutoCommit();
    conn_.Error();
    batch_params_.pop_front();
}

void SqlitePreparedStatement::SetString(int index, const std::string value) {
    SetValue(index, Value(in_place_type_t<std::string>{}, value));
}

void SqlitePreparedStatement::SetBoolean(int index, bool value) {
    SetValue(index, Value(in_place_type_t<bool>{}, value));
}

void SqlitePreparedStatement::SetShort(int index, short value) {
    SetValue(index, Value(in_place_type_t<short>{}, value));
}

void SqlitePreparedStatement::SetInt(int index, int value) {
    SetValue(index, Value(in_place_type_t<int>{}, value));
}

void SqlitePreparedStatement::SetLong(int index, long long value) {
    SetValue(index, Value(in_place_type_t<long long>{}, value));
}

void SqlitePreparedStatement::SetFloat(int index, float value) {
    SetValue(index, Value(in_place_type_t<float>{}, value));
}

void SqlitePreparedStatement::SetDouble(int index, double value) {
    SetValue(index, Value(in_place_type_t<double>{}, value));
}

void SqlitePreparedStatement::SetNull(int index) {
    SetValue(index, Value(in_place_type_t<std::nullptr_t>{}, nullptr));
}

void SqlitePreparedStatement::SetBlob(int index, Blob value) {
    SetValue(index, Value(in_place_type_t<Blob>{}, value));
}

void SqlitePreparedStatement::AddBatch() { batch_params_.emplace_front(); }

void SqlitePreparedStatement::ClearParameters() { GetParam()->clear(); }

void SqlitePreparedStatement::ClearBatch() { batch_params_.clear(); }

void SqlitePreparedStatement::ExecuteBatch() {
    while (batch_params_.size() > 1) {
        Bind(std::prev(batch_params_.end()));
        if (stmt_.Step() != SQLITE_DONE) {
            break;
        }
        batch_params_.pop_back();
    }
    conn_.EnsureAutoCommit();
    conn_.Error();
}

std::list<std::vector<Value>>::iterator SqlitePreparedStatement::GetParam() {
    auto param = batch_params_.begin();
    if (param->empty()) {
        for (int i = 0; i < stmt_.column_bind_count(); i++) {
            param->emplace_back(
                Value(in_place_type_t<std::nullptr_t>{}, nullptr));
        }
    }
    return param;
}

void SqlitePreparedStatement::SetValue(int index, Value&& value) {
    auto it = GetParam();
    it->at(index - 1) = std::move(value);
}

void SqlitePreparedStatement::Bind(
    std::list<std::vector<Value>>::iterator itr) {
    if (stmt_.column_bind_count() < itr->size())
        throw SQLException("param count miss!");

    for (int i = 0; i < itr->size(); i++) {
        if (!stmt_.BindParameter(i + 1, itr->at(i).Access())) {
            conn_.Error();
        }
    };
}

}  // namespace sql