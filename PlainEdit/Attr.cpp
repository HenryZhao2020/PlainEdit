#include "Attr.h"

QString Attr::recentDir;
QList<QString> Attr::recentPaths;

QString Attr::findTarget;
QString Attr::replaceTarget;
bool Attr::matchCase;
bool Attr::matchWholeWord;

bool Attr::wrapWord = true;

QString Attr::fontFamily;
int Attr::fontSize = 12;
QFont::Weight Attr::fontWeight;

int Attr::zoomPercent = 100;

void Attr::save() {
    QFile file("../Saved");
    file.open(QFile::WriteOnly);

    QDataStream out(&file);
    out << recentDir;
    out << recentPaths;

    out << findTarget;
    out << replaceTarget;
    out << matchCase;
    out << matchWholeWord;

    out << wrapWord;

    out << fontFamily;
    out << fontSize;
    out << fontWeight;

    out << zoomPercent;

    file.close();
}

bool Attr::load() {
    QFile file("../Saved");
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);
    in >> recentDir;
    in >> recentPaths;

    in >> findTarget;
    in >> replaceTarget;
    in >> matchCase;
    in >> matchWholeWord;

    in >> wrapWord;

    in >> fontFamily;
    in >> fontSize;
    in >> fontWeight;

    in >> zoomPercent;

    file.close();
    return true;
}
