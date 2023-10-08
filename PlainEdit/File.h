#pragma once

#include <QList>

namespace File {
    QString readAll(const QString &path);
    QList<QString> readLines(const QString &path);

    void write(const QString &path, const QString &text);
    void writeLines(const QString &path, const QList<QString> &list);
    void append(const QString &path, const QString &text);
}
