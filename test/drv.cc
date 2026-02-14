#include <iostream>

#include "sql/manager.h"

int test_jdbc_statement() {
    try {
        sql::DriverManager::LoadDriver("sqlite");
        sql::Connection conn = sql::DriverManager::GetConnection(
            "jdbc:sqlite:test.db", nullptr, nullptr);
        if (conn == nullptr) {
            std::cerr << "Failed to get connection" << std::endl;
            return -1;
        }

        conn->set_auto_commit(false);

        auto stmt = conn->CreateStatement();

        stmt->AddBatch(
            "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name "
            "TEXT, price REAL);");
        stmt->AddBatch(
            "INSERT INTO test (name, price) VALUES ('Alice01', 97.50);");
        stmt->AddBatch(
            "INSERT INTO test (name, price) VALUES ('Alice02', 88);");
        stmt->AddBatch(
            "INSERT INTO test (name, price) VALUES ('Alice03', 92.36);");
        stmt->ExecuteBatch();

        auto& rs = stmt->ExecuteQuery("select * from test");

        while (rs->NextRow()) {
            std::cout << rs->GetInteger(1).to_int64() << ":"
                      << rs->GetString("name").get() << ","
                      << rs->GetDecimal(3).to_double() << std::endl;
        }

        conn->Commit();

    } catch (const sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

int main() { return test_jdbc_statement(); }
