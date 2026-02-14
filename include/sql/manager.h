/**
 * @file manager.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the DriverManager class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef SQL_MANAGER_H
#define SQL_MANAGER_H

#include <memory>
#include <string>

#include "driver.h"
#include "exception.h"
#include "platform.h"

namespace sql {

/**
 * @class DriverManager
 *
 * This class is used to manage database connections and drivers. It provides a
 * mechanism for loading drivers and getting connections to the database.
 *
 * @brief A class that provides a way to manage database connections and
 *        drivers.
 * @note The class is a singleton and the only instance of the class can be
 *       retrieved using the GetInstance() method.
 */
class DriverManager {
   public:
    /**
     * @brief Get the singleton instance of the DriverManager class.
     *
     * This method returns the singleton instance of the DriverManager class.
     *
     * @return The singleton instance of the DriverManager class.
     */
    static DriverManager& GetInstance() {
        static DriverManager instance;
        return instance;
    }

    /**
     * @brief Get a connection to the database.
     *
     * This method returns a connection to the database using the specified URL,
     * username, and password.
     *
     * @param url The URL of the database.
     * @param username The username to use for the connection.
     * @param password The password to use for the connection.
     * @return A connection object representing the connection to the database.
     */
    static Connection GetConnection(const char* url, const char* username,
                                    const char* password) noexcept {
        return DriverManager::GetInstance().GetConnectionInternal(url, username,
                                                                  password);
    }

    /**
     * @brief Load a driver for the database.
     *
     * This method loads a driver for the database with the specified name.
     *
     * @param drvname The name of the driver to load.
     */
    static void LoadDriver(const char* drvname) {
        DriverManager::GetInstance().LoadDriverInternal(drvname);
    }

   private:
    DriverManager() : driver_(nullptr) { ; }

    /**
     * @brief Get a connection to the database.
     *
     * This method returns a connection to the database using the specified URL,
     * username, and password.
     *
     * @param url The URL of the database.
     * @param username The username to use for the connection.
     * @param password The password to use for the connection.
     * @return A connection object representing the connection to the database.
     */
    Connection GetConnectionInternal(const char* url, const char* username,
                                     const char* password) noexcept {
        return driver_ == nullptr
                   ? nullptr
                   : driver_->GetConnection(url, username, password);
    }

    /**
     * @brief Load a driver for the database.
     *
     * This method loads a driver for the database with the specified name.
     *
     * @param drvname The name of the driver to load.
     */
    void LoadDriverInternal(const std::string& drvname) {
        std::string driver_name = "./libdrv-" + drvname + SUFFIX;
        DL_HANDLE handle = DL_OPEN(driver_name.c_str());
        if (!handle) throw SQLException("load driver failed!");

        DriverBase* (*CreateDriver)() =
            (DriverBase * (*)()) DL_SYM(handle, "CreateDriver");
        if (!CreateDriver) {
            DL_CLOSE(handle);
            throw SQLException("init driver failed!");
        }

        driver_.reset(CreateDriver());
    }

   private:
    std::unique_ptr<DriverBase> driver_;  ///< driver
};
}  // namespace sql

#endif