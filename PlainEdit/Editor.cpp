#include "Editor.h"
#include "MainWindow.h"
#include "Attr.h"

Editor::Editor(MainWindow *window) : QPlainTextEdit(window) {
    this->window = window;

    auto newPalette = palette();
    newPalette.setColor(QPalette::Inactive, QPalette::Highlight,
                        newPalette.color(QPalette::Active,
                                         QPalette::Highlight));
    newPalette.setColor(QPalette::Inactive, QPalette::HighlightedText,
                        newPalette.color(QPalette::Active,
                                         QPalette::HighlightedText));
    setPalette(newPalette);
}

void Editor::setWordWrapped(bool wrapped) {
    if (wrapped) {
        setWordWrapMode(QTextOption::WordWrap);
    } else {
        setWordWrapMode(QTextOption::NoWrap);
    }
}

void Editor::removeSelectedText() {
    auto cursor = textCursor();
    if (cursor.hasSelection()) {
        cursor.removeSelectedText();
    }
}

void Editor::goTo(int line, int column) {
    auto cursor = textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, column);
    setTextCursor(cursor);
}

QTextCursor Editor::findPrevious() {
    int start = textCursor().position() - textCursor().selectedText().size();
    auto flags = searchFlags() | QTextDocument::FindBackward;
    auto cursor = document()->find(Attr::findTarget, start, flags);

    if (!cursor.isNull()) {
        setTextCursor(cursor);
        return cursor;
    }

    start = toPlainText().size() - 1;
    cursor = document()->find(Attr::findTarget, start, flags);

    if (!cursor.isNull()) {
        setTextCursor(cursor);
    }

    return cursor;
}

QTextCursor Editor::findNext() {
    int start = textCursor().position();
    auto flags = searchFlags();
    auto cursor = document()->find(Attr::findTarget, start, flags);

    if (!cursor.isNull()) {
        setTextCursor(cursor);
        return cursor;
    }

    start = 0;
    cursor = document()->find(Attr::findTarget, start, flags);

    if (!cursor.isNull()) {
        setTextCursor(cursor);
    }

    return cursor;
}

void Editor::replace() {
    auto cursor = findNext();
    if (cursor.isNull()) {
        QMessageBox::critical(this, "PlainEdit",
                              "Cannot find \"" + Attr::findTarget + "\"!");
        return;
    }

    int length = Attr::findTarget.length();
    int start = cursor.position() - length;

    cursor.setPosition(start);
    cursor.setPosition(start + length, QTextCursor::KeepAnchor);
    cursor.insertText(Attr::replaceTarget);
    cursor.setPosition(start, QTextCursor::KeepAnchor);
    setTextCursor(cursor);
}

void Editor::replaceAll() {
    auto cursor = findNext();
    if (cursor.isNull()) {
        QMessageBox::critical(this, "PlainEdit",
                              "Cannot find \"" + Attr::findTarget + "\"!");
        return;
    }

    while (!cursor.isNull()) {
        int length = Attr::findTarget.length();
        int start = cursor.position() - length;

        cursor.setPosition(start);
        cursor.setPosition(start + length, QTextCursor::KeepAnchor);
        cursor.insertText(Attr::replaceTarget);

        cursor = findNext();
    }
}

void Editor::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void Editor::dropEvent(QDropEvent *event) {
    auto mimeData = event->mimeData();
    if (!mimeData->hasUrls()) {
        return;
    }

    for (auto &url : mimeData->urls()) {
        MainWindow::open(url.toLocalFile());
    }
}

QTextDocument::FindFlags Editor::searchFlags() {
    QTextDocument::FindFlags flags;
    if (Attr::matchCase) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (Attr::matchWholeWord) {
        flags |= QTextDocument::FindWholeWords;
    }
    return flags;
}

Highlighter::Highlighter(Editor *editor)
    : QSyntaxHighlighter(editor->document()) {
    this->editor = editor;
    format.setBackground(QColor(255, 255, 0, 90));
}

void Highlighter::updateTarget() {
    editor->blockSignals(true);

    if (Attr::matchWholeWord) {
        ex = QRegularExpression("\\b" + Attr::findTarget + "\\b");
    } else {
        ex = QRegularExpression(Attr::findTarget);
    }

    if (!Attr::matchCase) {
        ex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    }

    rehighlight();
    editor->blockSignals(false);
}

void Highlighter::highlightBlock(const QString &text) {
    auto matches = ex.globalMatch(text);

    while (matches.hasNext()) {
        auto match = matches.next();
        setFormat(match.capturedStart(), Attr::findTarget.size(), format);
    }
}
