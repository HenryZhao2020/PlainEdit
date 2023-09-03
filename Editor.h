#pragma once

#include "PCH.h"

class MainWindow;

class Editor : public QPlainTextEdit {
    Q_OBJECT

public:
    Editor(MainWindow *window);

    void setWordWrap(bool wrap);
    void removeSelectedText();
    void goTo(int line, int column);
    QTextCursor findPrevious();
    QTextCursor findNext();
    void replace();
    void replaceAll();

private:
    MainWindow *window;

    QTextDocument::FindFlags searchFlags();
};

class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    Highlighter(Editor *editor);

    void updateTarget();
    void highlightBlock(const QString &text);

private:
    Editor *editor;
    QRegularExpression ex;
    QTextCharFormat format;
};
