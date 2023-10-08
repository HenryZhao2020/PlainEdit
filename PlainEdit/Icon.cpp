#include "Icon.h"

#include <QIcon>
#include <QHash>

QHash<QString, QIcon> icons = {};

QIcon getIcon(const QString &iconName) {
    if (!icons.contains(iconName)) {
        icons[iconName] = QIcon(iconName);
    }
    return icons[iconName];
}
