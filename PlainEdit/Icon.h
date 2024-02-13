#pragma once

/**
 * @brief Contains icon utilities.
 */
namespace Icon {
    /**
     * @brief Loads the icon with the specified path.
     * @note This method lowers the memory consumption as
     * it prevents the duplication of identical icons.
     * @param path The icon file path.
     * @return The icon at the specified path.
     */
    const QIcon &load(const QString &path);
}
