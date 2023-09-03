#include "MenuBar.h"
#include "MainWindow.h"
#include "Editor.h"
#include "Dialog.h"
#include "Attr.h"

MenuBar::MenuBar(MainWindow *window) : QMenuBar(window) {
    this->window = window;
    editor = window->getEditor();

    auto fileMenu = addMenu("&File");
    fileMenu->addAction("&New", QKeySequence("Ctrl+N"), &MainWindow::newWindow);
    fileMenu->addAction("&Open...", QKeySequence("Ctrl+O"), [window] {
        window->open();
    });

    recentMenu = fileMenu->addMenu("Open &Recent");
    recentSeparator = recentMenu->addSeparator();
    clearRecentAction = recentMenu->addAction("Clear Recent",
                                              &MainWindow::clearRecent);
    clearRecentAction->setEnabled(!Attr::recentPaths.isEmpty());

    fileMenu->addAction("&Save", QKeySequence("Ctrl+S"), [window] {
        window->save();
    });
    fileMenu->addAction("Save &As", QKeySequence("Ctrl+Shift+S"), [window] {
        window->saveAs();
    });
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", [window] {
        window->close();
    });

    auto editMenu = addMenu("&Edit");
    connect(editMenu, &QMenu::aboutToShow, this, &MenuBar::toggleEditActions);
    undoAction = editMenu->addAction("&Undo", QKeySequence("Ctrl+Z"), [this] {
        editor->undo();
    });
    undoAction->setEnabled(false);
    connect(window->getEditor(), &Editor::undoAvailable,
            this, &MenuBar::toggleUndoAction);
    redoAction = editMenu->addAction("&Redo", QKeySequence("Ctrl+Y"), [this] {
        editor->redo();
    });
    redoAction->setEnabled(false);
    connect(window->getEditor(), &Editor::redoAvailable,
            this, &MenuBar::toggleRedoAction);
    editMenu->addSeparator();
    cutAction = editMenu->addAction("Cu&t", QKeySequence("Ctrl+X"), [this] {
        editor->cut();
    });
    copyAction = editMenu->addAction("&Copy", QKeySequence("Ctrl+C"), [this] {
        editor->copy();
    });
    pasteAction = editMenu->addAction("&Paste", QKeySequence("Ctrl+V"), [this] {
        editor->paste();
    });
    deleteAction = editMenu->addAction("De&lete", [this] {
        editor->removeSelectedText();
    });
    editMenu->addSeparator();
    editMenu->addAction("&Find...", QKeySequence("Ctrl+F"), [window] {
        auto dialog = new FindDialog(window);
        dialog->show();
    });
    editMenu->addAction("&Replace...", QKeySequence("Ctrl+H"), [window] {
        auto dialog = new ReplaceDialog(window);
        dialog->show();
    });
    editMenu->addSeparator();
    editMenu->addAction("Select &All", QKeySequence("Ctrl+A"), [this] {
        editor->selectAll();
    });
    editMenu->addAction("&Go To...", QKeySequence("Ctrl+G"), [window] {
        auto dialog = new GoToDialog(window);
        dialog->show();
    });

    auto viewMenu = addMenu("&View");
    connect(viewMenu, &QMenu::aboutToShow, this, [this] {
        wrapBox->setChecked(Attr::wordWrap);
    });

    wrapBox = viewMenu->addAction("&Word Wrap");
    wrapBox->setCheckable(true);
    wrapBox->setChecked(Attr::wordWrap);
    connect(wrapBox, &QAction::triggered, this, [this] {
        MainWindow::setWordWrap(wrapBox->isChecked());
    });

    auto zoomMenu = viewMenu->addMenu("&Zoom");
    zoomMenu->addAction("Zoom &In", QKeySequence("Ctrl+="),
                        &MainWindow::zoomIn);
    zoomMenu->addAction("Zoom &Out", QKeySequence("Ctrl+-"),
                        &MainWindow::zoomOut);
    zoomMenu->addAction("&Restore Default Zoom", QKeySequence("Ctrl+0"),
                        &MainWindow::restoreDefaultZoom);

    viewMenu->addAction("&Font...", [window] {
        window->askNewFont();
    });

    auto helpMenu = addMenu("&Help");
    helpMenu->addAction("&About PlainEdit", [window] {
        auto aboutDialog = new AboutDialog(window);
        aboutDialog->show();
    });
}

void MenuBar::addRecentAction(const QString &path) {
    auto action = new QAction(path, recentMenu);
    connect(action, &QAction::triggered, this, [path] {
        MainWindow::open(path);
    });
    recentMenu->insertAction(recentSeparator, action);

    clearRecentAction->setEnabled(true);
}

void MenuBar::clearRecentActions() {
    auto actions = recentMenu->actions().mid(0, Attr::recentPaths.size());
    for (auto &action : actions) {
        recentMenu->removeAction(action);
    }

    clearRecentAction->setEnabled(false);
}

void MenuBar::toggleUndoAction(bool state) {
    undoAction->setEnabled(state);
}

void MenuBar::toggleRedoAction(bool state) {
    redoAction->setEnabled(state);
}

void MenuBar::toggleEditActions() {
    auto selection = editor->textCursor().selectedText();
    cutAction->setEnabled(selection != "");
    copyAction->setEnabled(selection != "");
    pasteAction->setEnabled(editor->canPaste());
    deleteAction->setEnabled(selection != "");
}
