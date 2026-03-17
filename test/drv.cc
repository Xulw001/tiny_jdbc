#include <iostream>

#include "sql/manager.h"
#include "sql/value.h"

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

        conn->Commit();

    } catch (const sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

int test_statement_query() {
    try {
        sql::DriverManager::LoadDriver("sqlite");
        sql::Connection conn = sql::DriverManager::GetConnection(
            "jdbc:sqlite:test.db", nullptr, nullptr);
        if (conn == nullptr) {
            std::cerr << "Failed to get connection" << std::endl;
            return -1;
        }

        auto stmt = conn->CreateStatement();
        auto& rs = stmt->ExecuteQuery("select * from test");
        while (rs->NextRow()) {
            std::cout << rs->GetInteger(1).to_int64() << ": "
                      << rs->GetString("name").get() << ", "
                      << rs->GetDecimal(3).to_double() << std::endl;
        }

    } catch (const sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

int test_prepared_statement() {
    try {
        sql::DriverManager::LoadDriver("sqlite");
        sql::Connection conn = sql::DriverManager::GetConnection(
            "jdbc:sqlite:test.db?cache=shared&journal_mode=WAL", nullptr,
            nullptr);
        if (conn == nullptr) {
            std::cerr << "Failed to get connection" << std::endl;
            return -1;
        }

        auto stmt = conn->PrepareStatement(
            "insert into test (name, price) values (?, ?)");
        stmt->SetString(1, sql::Text("Alice011"));
        stmt->SetDecimal(2, sql::Decimal(97.00));
        stmt->AddBatch();

        stmt->SetString(1, sql::Text("Alice012"));
        stmt->SetDecimal(2, sql::Decimal(82.30));
        stmt->AddBatch();

        stmt->SetString(1, sql::Text("Alice013"));
        stmt->SetDecimal(2, sql::Decimal(91.58));
        stmt->AddBatch();

        stmt->ExecuteBatch();

    } catch (const sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

int test_prepared_query() {
    try {
        sql::DriverManager::LoadDriver("sqlite");
        sql::Connection conn = sql::DriverManager::GetConnection(
            "jdbc:sqlite:test.db?cache=shared&journal_mode=WAL", nullptr,
            nullptr);
        if (conn == nullptr) {
            std::cerr << "Failed to get connection" << std::endl;
            return -1;
        }

        auto query =
            conn->PrepareStatement("select * from test where price > ?");
        query->SetDecimal(1, sql::Decimal(93.50));
        auto& rs = query->ExecuteQuery();
        while (rs->NextRow()) {
            std::cout << rs->GetInteger(1).to_int64() << " :"
                      << rs->GetString("name").get() << ", "
                      << rs->GetDecimal(3).to_double() << std::endl;
        }

        query->SetDecimal(1, sql::Decimal(90.50));
        auto& rs1 = query->ExecuteQuery();
        while (rs1->NextRow()) {
            std::cout << rs1->GetInteger(1).to_int64() << " :"
                      << rs1->GetString("name").get() << ", "
                      << rs1->GetDecimal(3).to_double() << std::endl;
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

int test_savepoint() {
    try {
        sql::DriverManager::LoadDriver("sqlite");
        sql::Connection conn = sql::DriverManager::GetConnection(
            "jdbc:sqlite:test.db?cache=shared&journal_mode=WAL", nullptr,
            nullptr);
        if (conn == nullptr) {
            std::cerr << "Failed to get connection" << std::endl;
            return -1;
        }

        conn->set_auto_commit(false);

        auto stmt = conn->CreateStatement();

        auto sp1 = conn->SetSavepoint("sp1");

        stmt->ExecuteUpdate(
            "CREATE TABLE test_savepoint (id INTEGER PRIMARY KEY, name TEXT);");

        stmt->ExecuteUpdate(
            "INSERT INTO test_savepoint (name) VALUES ('Alice01');");

        auto sp2 = conn->SetSavepoint("sp2");

        stmt->ExecuteUpdate(
            "INSERT INTO test_savepoint (name) VALUES ('Alice02');");

        stmt->ExecuteUpdate("drop table test_savepoint;");

        conn->Rollback(sp2);

        stmt->ExecuteUpdate(
            "INSERT INTO test_savepoint (name) VALUES ('Alice03');");

        auto sp3 = conn->SetSavepoint("sp3");

        conn->Commit();

        auto& rs = stmt->ExecuteQuery("select count(*) from test_savepoint");
        if (rs->NextRow()) {
            std::cout << "count: " << rs->GetInteger(1).to_int64() << std::endl;
        }

    } catch (const sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

int main() {
    test_jdbc_statement();
    test_prepared_statement();
    test_statement_query();
    test_prepared_query();
    test_savepoint();
    return 0;
}
