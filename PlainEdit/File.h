#pragma once

/**
 * @brief Contains file utilities.
 */
namespace File {
    /**
     * @brief Reads the file content.
     * @param path The file path.
     * @return The file content.
     */
    QString readAll(const QString &path);

    /**
     * @brief Writes the specified text to a file.
     * @param path The file path.
     * @param text The text to be written.
     */
    void writeAll(const QString &path, const QString &text);
}
