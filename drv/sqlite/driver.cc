#include "driver.h"

#include "connection.h"
#include "sql/exception.h"
#include "sql/platform.h"

namespace sql {

Connection SqliteDriver::GetConnection(const char* url, const char* username,
                                       const char* password) {
    std::string path, param;
    if (!ParseUrl(url, path, param)) {
        throw SQLException("invalid sqlite url!");
    }
    if (password) {
        param.append("pragma key=" + std::string(password) + ";");
    }
    return Connection(new SqliteConnection(path.c_str(), param.c_str()));
};

bool SqliteDriver::ParseUrl(const std::string& url, std::string& path,
                            std::string& params) {
    // jdbc:sqlite:<path>?parm1=value1&param2=value2
    const std::string prefix = "jdbc:sqlite:";
    if (url.compare(0, prefix.length(), prefix) != 0) {
        return false;
    }

    size_t pos = url.find('?', prefix.length());
    if (pos == std::string::npos) {
        path = url.substr(prefix.length());
        return true;
    }

    path = url.substr(prefix.length(), pos - prefix.length());
    // Parse parameters
    size_t start = pos + 1;
    while (start < url.length()) {
        size_t end = url.find('&', start);
        if (end == std::string::npos) {
            params.append("pragma " + url.substr(start) + ";");
            break;
        } else {
            params.append("pragma " + url.substr(start, end - start) + ";");
            start = end + 1;
        }
    }
    return true;
}
}  // namespace sql

/**
 * @brief Create a new SqliteDriver object.
 * @return A new SqliteDriver object.
 */
EXPORT_C sql::DriverBase* CreateDriver() { return new sql::SqliteDriver(); }