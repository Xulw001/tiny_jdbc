/**
 * @file value.h
 * @author xulw (nevermore.xulw@hotmail.com)
 * @brief This file defines the value class for SQL operations
 * @version 0.1
 * @date 2026-02-14
 *
 * @copyright Copyright (c) 2026
 */
#ifndef SQL_VALUE_H
#define SQL_VALUE_H

#include <string.h>

#include <memory>

#include "type/boolean.h"
#include "type/decimal.h"
#include "type/integer.h"
#include "type/text.h"

namespace sql {

enum class ValueType { NULLVAL = 0, INTEGER, REAL, TEXT, BLOB };

using reflect::Boolean;
using reflect::Decimal;
using reflect::Integer;
using reflect::ObjectInternal;
using reflect::Text;

/**
 * @struct Blob
 * @brief A class that represents a blob value in SQL operations.
 *
 * A blob value is a binary large object that stores data in a binary format.
 */
struct Blob : public ObjectInternal {
   public:
    /**
     * @brief Constructs a Blob object from a void pointer and its size.
     * @param[in] v The pointer to the data.
     * @param[in] n The size of the data in bytes.
     */
    explicit Blob(const void* v, int n) : size_(n + 1) {
        blob_.reset(new uint8_t[size_]);
        memcpy(blob_.get(), v, size_ - 1);
    }

    /**
     * @brief Gets the size of the blob value in bytes.
     * @return The size of the blob value.
     */
    int size() const { return size_; }

    /**
     * @brief Gets a pointer to the data of the blob value.
     * @return A pointer to the data of the blob value.
     */
    void* ptr() const { return blob_.get(); }

   private:
    int size_;  ///< The size of the blob value in bytes.
    std::shared_ptr<uint8_t>
        blob_;  ///< A shared pointer to the data of the blob value.
};
}  // namespace sql

#endif