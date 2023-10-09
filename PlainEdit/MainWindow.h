#pragma once

#include "PCH.h"

class Editor;
class MenuBar;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    MainWindow(const QString &path);

    static QList<MainWindow *> getWindows();
    Editor *getEditor();
    MenuBar *getMenuBar();

    static void newWindow();
    void open();
    static void open(const QString &path);
    static void addRecent(const QString &path);
    static void clearRecent();
    void save();
    void save(const QString &path);
    void saveAs();
    static void setWordWrap(bool wrap);
    void askNewFont();
    static void setFont(const QFont &font);
    static void setZoomPercent(int percent);
    static void zoomIn();
    static void zoomOut();
    static void restoreDefaultZoom();
    void bringToFront();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    static QList<MainWindow *> windows;

    QString path;
    QString title;
    bool saved;

    Editor *editor;
    MenuBar *menuBar;

    void updateTitle();
};
