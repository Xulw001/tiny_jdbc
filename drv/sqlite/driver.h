/**
 * @file driver.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the SqliteDriver class
 * @version 0.1
 * @date 2026-02-14
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
 *
 * This class provides a pure virtual interface for database drivers.
 * It is designed to be used by a derived class that provides a specific
 * implementation for a particular database management system.
 *
 * @brief A class that implements the DriverBase interface for SQLite.
 */
class SqliteDriver : public DriverBase {
   public:
    virtual Connection GetConnection(const char* url, const char* username,
                                     const char* password) override;

   private:
    bool ParseUrl(const std::string& url, std::string& path,
                  std::string& params);
};
}  // namespace sql

#endif