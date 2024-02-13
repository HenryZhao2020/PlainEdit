#include "Icon.h"

// Store created icons
QHash<QString, QIcon> cache;

const QIcon &Icon::load(const QString &path) {
    auto it = cache.constFind(path);
    if (it == cache.constEnd()) {
        it = cache.insert(path, QIcon(path));
    }
    return it.value();
}
