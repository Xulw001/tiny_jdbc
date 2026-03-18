/**
 * @file savepoint.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief SQLite implementation of savepoint
 * @version 0.2
 * @date 2026-03-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef DRV_SQLITE_SAVEPOINT_H
#define DRV_SQLITE_SAVEPOINT_H

#include "sql/savepoint.h"

namespace sql {
/**
 * @class SqliteSavepoint
 * @brief SQLite implementation of SavepointBase
 * @details Represents a savepoint in a SQLite transaction
 */
class SqliteSavepoint : public SavepointBase {
   public:
    /**
     * @brief Construct a new SqliteSavepoint object
     * @param name Savepoint name
     */
    SqliteSavepoint(const std::string& name) : name_(name) { ; }

    virtual ~SqliteSavepoint() = default;

    /**
     * @brief Get the name of the savepoint
     * @return Savepoint name
     */
    virtual const std::string& name() const override { return name_; }

   private:
    std::string name_;  ///< The name of the savepoint
};

}  // namespace sql
#endif