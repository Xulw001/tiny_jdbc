/**
 * @file exception.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the exception class for SQL operations
 * @version 0.1
 * @date 2026-02-10
 *
 * @copyright Copyright (c) 2026
 */
#ifndef SQL_EXCEPTION_H
#define SQL_EXCEPTION_H

#include <string>

namespace sql {

/**
 * @brief Exception class for SQL operations
 */
class SQLException {
   public:
    SQLException(const std::string& message) noexcept : message_(message) {}
    virtual ~SQLException() = default;
    std::string what() const { return message_; }

   private:
    std::string message_;
};

}  // namespace sql

#endif