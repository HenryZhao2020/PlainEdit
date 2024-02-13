#include "MainWindow.h"
#include "MenuBar.h"
#include "Editor.h"
#include "StatusBar.h"
#include "Attr.h"

QList<MainWindow *> MainWindow::windows;
const QString MainWindow::EXT_FILTER =
    QFileDialog::tr("Text Documents (*.txt)") + "\n" +
    QFileDialog::tr("All Files (*.*)");

MainWindow::MainWindow(const QString &path) {
    // Get the full file path
    filePath = QFileInfo(path).absoluteFilePath();
    // The default file name is 'Untitled' if no file is opened
    fileName = filePath.isEmpty() ? tr("Untitled") : QFileInfo(filePath).fileName();
    // If the specified path does not exist, treat the file as 'unsaved'
    saved = filePath.isEmpty() ? true : QFileInfo(filePath).exists();

    // Register this instance
    windows.append(this);

    resize(1080, 720);
    setAttribute(Qt::WA_DeleteOnClose);

    // Place an editor in the center
    editor = new Editor(this);
    if (!filePath.isEmpty()) {
        editor->setPlainText(File::readAll(filePath));
    }
    connect(editor, &Editor::textChanged, this, &MainWindow::updateSave);
    setCentralWidget(editor);

    // Place a menu bar on the top
    menuBar = new MenuBar(this);
    setMenuBar(menuBar);

    // Place a status bar on the bottom
    statusBar = new StatusBar(this);
    setStatusBar(statusBar);

    // Display this window
    updateTitle();
    show();
    WinGui::raiseWidget(this);
}

MainWindow::~MainWindow() {

}

Editor *MainWindow::getEditor() {
    return editor;
}

MenuBar *MainWindow::getMenuBar() {
    return menuBar;
}

StatusBar *MainWindow::getStatusBar() {
    return statusBar;
}

void MainWindow::open() {
    // Prompt the user to select file(s) to open
    const QStringList &paths = QFileDialog::getOpenFileNames(
        this, QFileDialog::tr("Open"), Attr::get().recentDir, EXT_FILTER);

    // Open each file in a separate window
    for (const auto &path : paths) {
        open(path);

        // Update the recent directory
        Attr::get().recentDir = QFileInfo(path).absolutePath();
    }
}

bool MainWindow::save() {
    // If no file is opened, choose a location to save the editor content
    if (filePath.isEmpty()) {
        saveAs();
    // Otherwise, save the file at the current path
    } else {
        save(filePath);
    }

    return saved;
}

void MainWindow::saveAs() {
    // Prompt the user to select where to save the file
    const QString &path = QFileDialog::getSaveFileName(
        this, QFileDialog::tr("Save As"), Attr::get().recentDir, EXT_FILTER);

    // Exit the function if the user closes the file dialog
    if (path.isEmpty()) {
        return;
    }

    // Get the full file path
    const QString &fullPath = QFileInfo(path).absoluteFilePath();

    // Update the recent directory
    Attr::get().recentDir = QFileInfo(path).absolutePath();

    // If the selected path is the same as the original path,
    // save the current file
    if (fullPath == filePath) {
        save(fullPath);
        return;
    }

    // If a file is already opened in another window, close that window
    for (const auto &win : windows) {
        if (win->filePath == fullPath) {
            win->close();
            break;
        }
    }

    filePath = fullPath;
    fileName = QFileInfo(fullPath).fileName();
    addRecent(filePath);
    save(filePath);
}

void MainWindow::selectNewFont() {
    bool ok;
    const auto &font = QFontDialog::getFont(
        &ok, Attr::get().editorFont, this, QFontDialog::tr("Select Font"));

    if (ok) {
        Attr::get().editorFont = font;
    } else {
        return;
    }

    // Update the editor font of every window
    for (const auto &win : windows) {
        win->getEditor()->setFont(font);
        win->getEditor()->getLineBar()->setFont(font);
    }
}

void MainWindow::newWindow() {
    new MainWindow();
}

void MainWindow::open(const QString &path) {
    // Get the full file path
    const QString &fullPath = QFileInfo(path).absoluteFilePath();

    // If a file is already opened in another window, switch to that window
    for (const auto &win : windows) {
        if (win->filePath == fullPath) {
            WinGui::raiseWidget(win);
            return;
        }
    }

    addRecent(fullPath);
    new MainWindow(fullPath);
}

void MainWindow::addRecent(const QString &path) {
    // Get the full file path
    const QString &fullPath = QFileInfo(path).absoluteFilePath();

    // Ensure there are no duplicate paths
    if (Attr::get().recentPaths.contains(fullPath)) {
        return;
    }

    // Update the recent menu of every window
    for (const auto &win : windows) {
        win->menuBar->addRecent(fullPath);
    }

    Attr::get().recentPaths.append(fullPath);
}

void MainWindow::clearRecent() {
    // Update the recent menu of every window
    for (const auto &win : windows) {
        win->menuBar->clearRecent();
    }

    Attr::get().recentPaths = {};
}

void MainWindow::showLineNum(bool shown) {
    Attr::get().showLine = shown;

    for (const auto &win : windows) {
        win->getEditor()->updateLineBarWidth();
    }
}

void MainWindow::showStatus(bool shown) {
    Attr::get().showStatus = shown;

    for (const auto &win : windows) {
        win->getStatusBar()->setVisible(shown);
    }
}

void MainWindow::setWordWrap(bool wrap) {
    Attr::get().wordWrap = wrap;

    for (const auto &win : windows) {
        win->getEditor()->setWordWrap(wrap);
    }
}

void MainWindow::zoomIn() {
    setZoom(Attr::get().zoom + 10);
}

void MainWindow::zoomOut() {
    setZoom(Attr::get().zoom - 10);
}

void MainWindow::resetZoom() {
    setZoom(100);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    QMainWindow::dragEnterEvent(event);

    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    QMainWindow::dropEvent(event);

    const auto &mimeData = event->mimeData();
    if (!mimeData->hasUrls()) {
        return;
    }

    for (const auto &url : mimeData->urls()) {
        MainWindow::open(url.toLocalFile());
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);

    // If the file is already saved, close the window without confirmation
    if (saved) {
        windows.removeOne(this);
        event->accept();
        return;
    }

    // If the file is unsaved, prompt the user to save it
    const auto &ans = QMessageBox::question(
        this, tr("Confirm Exiting"),
        tr("Do you want to save changes to %0?").arg(fileName),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    // If the user selects 'Yes' and the saving process is successful,
    // or if the user selects 'No', close this window
    if ((ans == QMessageBox::Yes && save()) || ans == QMessageBox::No) {
        windows.removeOne(this);
        event->accept();
    // If the saving is interrupted, or if the user selects 'Cancel',
    // this window will not be closed
    } else {
        event->ignore();
    }
}

void MainWindow::save(const QString &path) {
    // Skip the operation if the file is already saved
    if (saved) {
        return;
    }

    // Save the file
    File::writeAll(path, editor->toPlainText());

    saved = true;
    updateTitle();
}

void MainWindow::updateSave() {
    // If no file is opened, treat the file as 'saved' if the editor is empty
    // Otherwise, the file is unsaved
    saved = filePath.isEmpty() ? editor->toPlainText().isEmpty() : false;
    updateTitle();
}

void MainWindow::updateTitle() {
    QString title = fileName + " - " + tr("PlainEdit");
    // Unsaved file starts with an asterisk symbol (*)
    if (!saved) {
        title = "*" + title;
    }
    setWindowTitle(title);
}

void MainWindow::setZoom(int zoom) {
    // Ensure the zoom percentage is between 10% and 500%
    if (zoom > 500 || zoom < 10) {
        return;
    }

    Attr::get().zoom = zoom;

    // Update the zoom percentage of every window
    for (const auto &win : windows) {
        win->getEditor()->setZoom(zoom);
        win->getStatusBar()->updateZoom();
    }
}
