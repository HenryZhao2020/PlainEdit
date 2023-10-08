#pragma once

#include "PCH.h"

class MainWindow;
class Editor;

class MenuBar : public QMenuBar {
    Q_OBJECT

public:
    MenuBar(MainWindow *window);

    void addRecentAction(const QString &path);
    void clearRecentActions();

private:
    MainWindow *window;
    Editor *editor;

    QMenu *recentMenu;
    QAction *recentSeparator;
    QAction *clearRecentAction;

    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;

    QAction *wrapBox;

    void toggleUndoAction(bool state);
    void toggleRedoAction(bool state);
    void toggleEditActions();
};
