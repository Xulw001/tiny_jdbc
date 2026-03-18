#include "stmt.h"

#include "sql/exception.h"
#include "type_id.h"

namespace sql {

int SqliteStmt::Step() {
    int ret = sqlite3_step(stmt_.get());
    if (ret == SQLITE_DONE) {
        sqlite3_reset(stmt_.get());
        sqlite3_clear_bindings(stmt_.get());
    }
    return ret;
}

void SqliteStmt::UpdateColumn() {
    col_count_ = sqlite3_column_count(stmt_.get());
    for (int i = 0; i < col_count_; i++) {
        col_map_.emplace(sqlite3_column_name(stmt_.get(), i), i);
    };
}

int SqliteStmt::GetIndexByName(const std::string& col_name) {
    auto it = col_map_.find(col_name);
    if (it == col_map_.end()) throw SQLException("unknown column name!");
    return it->second;
}

int SqliteStmt::column_bind_count() {
    return sqlite3_bind_parameter_count(stmt_.get());
}

bool SqliteStmt::BindParameter(int index, Reference ref) {
    if (ref.type_id() == reflect::TypeEnum::CPPTYPE_BOOL) {
        if (SQLITE_OK !=
            sqlite3_bind_int(stmt_.get(), index, ref.cast<bool>())) {
            return false;
        }
    } else if (ref.type_id() == reflect::TypeEnum::CPPTYPE_FLOAT) {
        if (SQLITE_OK !=
            sqlite3_bind_double(stmt_.get(), index, ref.cast<float>())) {
            return false;
        }
    } else if (ref.type_id() == reflect::TypeEnum::CPPTYPE_DOUBLE) {
        if (SQLITE_OK !=
            sqlite3_bind_double(stmt_.get(), index, ref.cast<double>())) {
            return false;
        }
    } else if (ref.type_id() == reflect::TypeEnum::CPPTYPE_STRING) {
        auto& str = ref.cast<std::string>();
        if (SQLITE_OK != sqlite3_bind_text(stmt_.get(), index, str.c_str(), -1,
                                           SQLITE_TRANSIENT)) {
            return false;
        }
    } else if (ref.type_id() == reflect::TypeEnum::CPPTYPE_INT16) {
        if (SQLITE_OK !=
            sqlite3_bind_int(stmt_.get(), index, ref.cast<short>())) {
            return false;
        }
    } else if (ref.type_id() == reflect::TypeEnum::CPPTYPE_INT32) {
        if (SQLITE_OK !=
            sqlite3_bind_int(stmt_.get(), index, ref.cast<int>())) {
            return false;
        }
    } else if (ref.type_id() == reflect::TypeEnum::CPPTYPE_INT64) {
        if (SQLITE_OK !=
            sqlite3_bind_int64(stmt_.get(), index, ref.cast<long long>())) {
            return false;
        }
    } else if (ref.type_id() == reflect::get_type_id<Blob>()) {
        auto& blob = ref.cast<Blob>();
        if (SQLITE_OK != sqlite3_bind_blob(stmt_.get(), index, blob.ptr(),
                                           blob.size(), SQLITE_TRANSIENT)) {
            return false;
        }
    } else if(ref.type_id() == reflect::get_type_id<std::nullptr_t>()){
        if (SQLITE_OK != sqlite3_bind_null(stmt_.get(), index)) {
            return false;
        }
    }
    return true;
}

Value SqliteStmt::GetValue(int index, ValueType type) {
    switch (sqlite3_column_type(stmt_.get(), index)) {
    case SQLITE_INTEGER:
        TypeCheck(SQLITE_INTEGER, type);
        return Value(in_place_type_t<long long>{},
                     sqlite3_column_int64(stmt_.get(), index));
    case SQLITE_FLOAT:
        TypeCheck(SQLITE_FLOAT, type);
        return Value(in_place_type_t<double>{},
                     sqlite3_column_double(stmt_.get(), index));
    case SQLITE_BLOB:
        TypeCheck(SQLITE_BLOB, type);
        return Value(in_place_type_t<Blob>{},
                     Blob(sqlite3_column_blob(stmt_.get(), index),
                          sqlite3_column_bytes(stmt_.get(), index)));
    case SQLITE_TEXT:
        TypeCheck(SQLITE_TEXT, type);
        return Value(in_place_type_t<std::string>{},
                     (char*)sqlite3_column_text(stmt_.get(), index));
    case SQLITE_NULL:
    default:
        TypeCheck(SQLITE_NULL, type);
        return Value(in_place_type_t<std::nullptr_t>{}, nullptr);
    }
    throw SQLException("invalid data type!");
}

void SqliteStmt::TypeCheck(int col_type, ValueType type) {
    if (col_type != static_cast<int>(type))
        throw SQLException("invalid data type!");
}

}  // namespace sql