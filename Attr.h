#pragma once

#include "PCH.h"

class Attr {
public:
    static QString recentDir;
    static QList<QString> recentPaths;

    static QString findTarget;
    static QString replaceTarget;
    static bool matchCase;
    static bool matchWholeWord;

    static bool wordWrap;

    static QString fontFamily;
    static int fontSize;
    static QFont::Weight fontWeight;

    static int zoomPercent;

    static void save();
    static bool load();
};
