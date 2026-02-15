#include "driver.h"

#include "connection.h"
#include "sql/exception.h"
#include "sql/platform.h"

namespace sql {
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

/**
 * @brief Parse the URL and extract the path and parameters.
 *
 * This function parses the URL and extracts the path and parameters.
 *
 * @param url The URL of the database.
 * @param path The path to the database.
 * @param params The parameters to the database.
 * @return True if the URL is parsed successfully, false otherwise.
 */
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