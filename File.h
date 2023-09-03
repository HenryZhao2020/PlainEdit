#pragma once

class File {
public:
    static QString readAll(const QString &path);
    static QList<QString> readLines(const QString &path);

    static void write(const QString &path, const QString &text);
    static void writeLines(const QString &path, const QList<QString> &list);
    static void append(const QString &path, const QString &text);
};
