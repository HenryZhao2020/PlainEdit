#include "File.h"

#include <QFile>
#include <QTextStream>

QString File::readAll(const QString &path) {
    return readLines(path).join("\n");
}

QList<QString> File::readLines(const QString &path) {
    QFile file(path);
    file.open(QFile::ReadOnly);

    QTextStream stream(&file);
    QList<QString> lines;

    while (!stream.atEnd()) {
        lines.append(stream.readLine());
    }

    file.close();
    return lines;
}

void File::write(const QString &path, const QString &text) {
    QFile file(path);
    file.open(QFile::Truncate | QFile::WriteOnly);
    file.write(text.toUtf8());
    file.close();
}

void File::writeLines(const QString &path, const QList<QString> &list) {
    write(path, list.join("\n"));
}

void File::append(const QString &path, const QString &text) {
    QFile file(path);
    file.open(QFile::Append);
    file.write(QString(text + "\n").toUtf8());
    file.close();
}
