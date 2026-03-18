/**
 * @file driver.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief SQLite implementation of driver
 * @version 0.2
 * @date 2026-03-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef DRV_SQLITE_DRIVER_H
#define DRV_SQLITE_DRIVER_H

#include <string>

#include "sql/driver.h"

namespace sql {
/**
 * @class SqliteDriver
 * @brief SQLite implementation of DriverBase
 * @details Implements driver operations for SQLite
 */
class SqliteDriver : public DriverBase {
   public:
    /**
     * @brief Create a database connection
     * @param url Database URL
     * @param username Database username (ignored for SQLite)
     * @param password Database password (ignored for SQLite)
     * @return Database connection object
     */
    virtual Connection GetConnection(const char* url, const char* username,
                                     const char* password) override;

   private:
    /**
     * @brief Parse SQLite URL
     * @details Parses SQLite URL into path and parameters
     * @param url SQLite URL
     * @param path Output parameter for database path
     * @param params Output parameter for URL parameters
     * @return True if URL parsed successfully, false otherwise
     */
    bool ParseUrl(const std::string& url, std::string& path,
                  std::string& params);
};
}  // namespace sql

#endif