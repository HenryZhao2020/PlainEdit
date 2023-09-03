#pragma once

class Pixmap {
public:
    static QPixmap &get(const QString &fileName);

private:
    static QHash<QString, QPixmap> pixmaps;
};
