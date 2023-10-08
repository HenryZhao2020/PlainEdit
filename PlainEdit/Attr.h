#pragma once

#include "PCH.h"

namespace Attr {
    extern QString recentDir;
    extern QList<QString> recentPaths;

    extern QString findTarget;
    extern QString replaceTarget;
    extern bool matchCase;
    extern bool matchWholeWord;

    extern bool wrapWord;

    extern QString fontFamily;
    extern int fontSize;
    extern QFont::Weight fontWeight;

    extern int zoomPercent;

    void save();
    bool load();
}
