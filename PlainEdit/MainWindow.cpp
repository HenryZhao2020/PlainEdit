#include "MainWindow.h"
#include "Editor.h"
#include "MenuBar.h"
#include "Attr.h"

const QString EXTENSION_FILTER = "Text Documents (*.txt)\nAll Files (*.*)";

QList<MainWindow *> MainWindow::windows = {};

MainWindow::MainWindow() {
    title = "Untitled";
    saved = true;
    windows.append(this);

    setWindowTitle(title + " - PlainEdit");
    resize(1080, 720);

    editor = new Editor(this);
    editor->setWordWrapped(Attr::wrapWord);
    connect(editor, &Editor::textChanged, this, &MainWindow::updateTitle);
    setCentralWidget(editor);

    int fontSize = Attr::fontSize + (Attr::zoomPercent - 100) / 10;
    if (fontSize > 0) {
        editor->setFont(QFont(Attr::fontFamily, fontSize, Attr::fontWeight));
    }

    menuBar = new MenuBar(this);
    for (auto &path : Attr::recentPaths) {
        menuBar->addRecentAction(path);
    }
    setMenuBar(menuBar);

    bringToFront();
}

MainWindow::MainWindow(const QString &path) : MainWindow() {
    this->path = path;
    title = QFileInfo(path).fileName();
    saved = QFileInfo::exists(path);

    if (saved) {
        setWindowTitle(title + " - PlainEdit");
    } else {
        setWindowTitle("*" + title + " - PlainEdit");
    }

    editor->blockSignals(true);
    editor->setPlainText(File::readAll(path));
    editor->blockSignals(false);
}

Editor *MainWindow::getEditor() {
    return editor;
}

MenuBar *MainWindow::getMenuBar() {
    return menuBar;
}

void MainWindow::newWindow() {
    auto window = new MainWindow();
    window->show();
}

void MainWindow::open() {
    auto paths = QFileDialog::getOpenFileNames(
        this, "Open", Attr::recentDir, EXTENSION_FILTER);

    for (auto &path : paths) {
        open(path);
    }
}

void MainWindow::open(const QString &path) {
    Attr::recentDir = QFileInfo(path).absolutePath();

    auto fullPath = QFileInfo(path).absoluteFilePath();
    addRecent(fullPath);

    for (auto &window : windows) {
        if (window->path == fullPath) {        
            window->bringToFront();
            return;
        }
    }

    auto window = new MainWindow(fullPath);
    window->show();
}

void MainWindow::addRecent(const QString &path) {
    if (Attr::recentPaths.contains(path)) {
        return;
    }

    for (auto &window : windows) {
        window->menuBar->addRecentAction(path);
    }

    Attr::recentPaths.append(path);
}

void MainWindow::clearRecent() {
    for (auto &window : windows) {
        window->menuBar->clearRecentActions();
    }

    Attr::recentPaths.clear();
}

void MainWindow::save() {
    if (path == "") {
        saveAs();
    } else {
        save(path);
    }
}

void MainWindow::save(const QString &path) {
    File::write(path, editor->toPlainText());

    saved = true;
    setWindowTitle(title + " - PlainEdit");    
}

void MainWindow::saveAs() {
    auto newPath = QFileDialog::getSaveFileName(
        this, "Save As", Attr::recentDir, EXTENSION_FILTER);

    if (newPath == "") {
        return;
    }

    for (auto &window : windows) {
        if (window->path == newPath) {
            window->close();
            break;
        }
    }

    path = newPath;
    title = QFileInfo(path).fileName();

    addRecent(path);
    save(path);
}

void MainWindow::setWordWrap(bool wrap) {
    Attr::wrapWord = wrap;

    for (auto &window : windows) {
        window->editor->setWordWrapped(wrap);
    }
}

void MainWindow::askNewFont() {
    bool ok;
    QFont oldFont(Attr::fontFamily, Attr::fontSize, Attr::fontWeight);
    auto newFont = QFontDialog::getFont(&ok, oldFont, this, "Font");

    if (!ok) {
        return;
    }

    Attr::fontFamily = newFont.family();
    Attr::fontSize = newFont.pointSize();
    Attr::fontWeight = newFont.weight();

    setFont(newFont);
}

void MainWindow::setFont(const QFont &font) {
    for (auto &window : windows) {
        window->editor->setFont(font);
    }
}

void MainWindow::setZoomPercent(int percent) {
    Attr::zoomPercent = percent;

    int fontSize = Attr::fontSize + (percent - 100) / 10;
    if (fontSize > 0) {
        setFont(QFont(Attr::fontFamily, fontSize, Attr::fontWeight));
    }
}

void MainWindow::zoomIn() {
    if (Attr::zoomPercent == 500) {
        return;
    }

    setZoomPercent(Attr::zoomPercent + 10);
}

void MainWindow::zoomOut() {
    if (Attr::zoomPercent == 10) {
        return;
    }

    setZoomPercent(Attr::zoomPercent - 10);
}

void MainWindow::restoreDefaultZoom() {
    setZoomPercent(100);
}

void MainWindow::bringToFront() {
    auto hwnd = (HWND) winId();
    if (IsIconic(hwnd)) {
        ShowWindow(hwnd, SW_RESTORE);
    }
    SetForegroundWindow(hwnd);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (!saved) {
        auto button = QMessageBox::question(
            this, "PlainEdit", "Do you want to save changes to this file?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (button == QMessageBox::Cancel) {
            event->ignore();
            return;
        }

        if (button == QMessageBox::Yes) {
            save();
            if (!saved) {
                event->ignore();
                return;
            }
        }
    }

    windows.removeOne(this);
    event->accept();
}

void MainWindow::updateTitle() {
    if (path == "" && editor->toPlainText() == "") {
        saved = true;
        setWindowTitle(title + " - PlainEdit");
    } else {
        saved = false;
        setWindowTitle("*" + title + " - PlainEdit");
    }
}
