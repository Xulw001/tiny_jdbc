/**
 * @file driver.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the DriverBase class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef SQL_DRV_H
#define SQL_DRV_H

#include "connection.h"

namespace sql {
/**
 * @brief Abstract base class for database drivers.
 *
 * This class provides a pure virtual interface for database drivers.
 * It is designed to be used by a derived class that provides a specific
 * implementation for a particular database management system.
 */
class DriverBase {
   public:
    /**
     * @brief Get a connection to the database.
     *
     * This function retrieves a connection to the database using the
     * specified URL, username, and password.
     *
     * @param url The URL of the database.
     * @param username The username to use for the connection.
     * @param password The password to use for the connection.
     * @return A connection object representing the connection to the database.
     */
    virtual Connection GetConnection(const char* url, const char* username,
                                     const char* password) = 0;
};
}  // namespace sql

#endif