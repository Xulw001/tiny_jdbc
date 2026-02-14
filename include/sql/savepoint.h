/**
 * @file savepoint.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the Savepoint class
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef SQL_SAVEPOINT_H
#define SQL_SAVEPOINT_H

#include <memory>
#include <string>

namespace sql {

/**
 * @brief Abstract base class for savepoint objects.
 *
 * This class is an abstract base class for savepoint objects. It provides
 * a pure virtual function to get the name of the savepoint.
 */
class SavepointBase {
   public:
    virtual ~SavepointBase() = default;
    /**
     * @brief Get the name of the savepoint.
     * @return The name of the savepoint.
     */
    virtual const std::string& name() const = 0;
};

using Savepoint = std::unique_ptr<SavepointBase>;

}  // namespace sql
#endif