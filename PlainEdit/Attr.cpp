#include "Attr.h"

const QString Lang::ENGLISH = "English";
const QString Lang::CHINESE_SIMPLIFIED = "简体中文";

Attr::Attr() {

}

Attr::~Attr() {

}

void Attr::save() {
    QFile file("../Saved");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    QDataStream out(&file);
    out << recentDir;
    out << recentPaths;

    out << findTarget;
    out << replaceTarget;
    out << matchCase;
    out << matchWholeWord;

    out << showLine;
    out << showStatus;
    out << wordWrap;
    out << zoom;

    out << editorFont;
    out << lang;

    file.close();
}

bool Attr::load() {
    QFile file("../Saved");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QDataStream in(&file);
    in >> recentDir;
    in >> recentPaths;

    in >> findTarget;
    in >> replaceTarget;
    in >> matchCase;
    in >> matchWholeWord;

    in >> showLine;
    in >> showStatus;
    in >> wordWrap;
    in >> zoom;

    in >> editorFont;
    in >> lang;

    file.close();
    return true;
}

Attr &Attr::get() {
    static Attr attr;
    return attr;
}
