#include "resultset.h"

#include <algorithm>

#include "sql/exception.h"

namespace sql {

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

std::string SqliteResultSet::GetString(int index) {
    auto value = stmt_.GetValue(index - 1, ValueType::TYPE_TEXT);
    has_value_ = value.HasValue();
    if (has_value_) {
        return value.Access().cast<std::string>();
    }
    return "";
}

std::string SqliteResultSet::GetString(const std::string& column_name) {
    return std::move(GetString(stmt_.GetIndexByName(column_name) + 1));
}

short SqliteResultSet::GetShort(int index) { return (short)GetLong(index); }

short SqliteResultSet::GetShort(const std::string& column_name) {
    return (short)GetLong(stmt_.GetIndexByName(column_name) + 1);
}

int SqliteResultSet::GetInt(int index) { return (int)GetLong(index); }

int SqliteResultSet::GetInt(const std::string& column_name) {
    return (int)GetLong(stmt_.GetIndexByName(column_name) + 1);
}

long long SqliteResultSet::GetLong(int index) {
    Value value = stmt_.GetValue(index - 1, ValueType::TYPE_INTEGER);
    has_value_ = value.HasValue();
    if (has_value_) {
        return value.Access().cast<long long>();
    }
    return 0;
}

long long SqliteResultSet::GetLong(const std::string& column_name) {
    return GetLong(stmt_.GetIndexByName(column_name) + 1);
}

float SqliteResultSet::GetFloat(int index) { return (float)GetDouble(index); }

float SqliteResultSet::GetFloat(const std::string& column_name) {
    return (float)GetDouble(stmt_.GetIndexByName(column_name) + 1);
}

double SqliteResultSet::GetDouble(int index) {
    Value value = stmt_.GetValue(index - 1, ValueType::TYPE_REAL);
    has_value_ = value.HasValue();
    if (has_value_) {
        return value.Access().cast<double>();
    }
    return 0.0;
}

double SqliteResultSet::GetDouble(const std::string& column_name) {
    return GetDouble(stmt_.GetIndexByName(column_name) + 1);
}

bool SqliteResultSet::GetBoolean(int index) {
    Value value = stmt_.GetValue(index - 1, ValueType::TYPE_INTEGER);
    has_value_ = value.HasValue();
    if (has_value_) {
        return value.Access().cast<long long>() != 0;
    }
    return false;
}

bool SqliteResultSet::GetBoolean(const std::string& column_name) {
    return GetBoolean(stmt_.GetIndexByName(column_name) + 1);
}

Blob SqliteResultSet::GetBlob(int index) {
    Value value = stmt_.GetValue(index - 1, ValueType::TYPE_BLOB);
    has_value_ = value.HasValue();
    if (has_value_) {
        return value.Access().cast<Blob>();
    }
    return Blob("", 0);
}

Blob SqliteResultSet::GetBlob(const std::string& column_name) {
    return std::move(GetBlob(stmt_.GetIndexByName(column_name) + 1));
}
};  // namespace sql