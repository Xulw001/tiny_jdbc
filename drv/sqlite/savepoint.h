/**
 * @file savepoint.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the SqliteSavepoint class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef DRV_SQLITE_SAVEPOINT_H
#define DRV_SQLITE_SAVEPOINT_H

#include "sql/savepoint.h"

namespace sql {

/**
 * @brief Savepoint class for SQLite.
 *
 * This class provides an implementation of the SavepointBase class for SQLite.
 */
class SqliteSavepoint : public SavepointBase {
   public:
    /**
     * @brief Constructor.
     * @param name The name of the savepoint.
     */
    SqliteSavepoint(const std::string& name) : name_(name) { ; }

    virtual ~SqliteSavepoint() = default;

    /**
     * @brief Get the name of the savepoint.
     * @return The name of the savepoint.
     */
    virtual const std::string& name() const override { return name_; }

   private:
    std::string name_;  ///< The name of the savepoint
};

}  // namespace sql
#endif