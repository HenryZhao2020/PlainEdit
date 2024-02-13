#pragma once

// Forward declarations
class MenuBar;
class Editor;
class StatusBar;

/**
 * @brief Displays primary UI elements, including a menu bar on the top,
 * an editor in the center, and a status bar on the bottom.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Provides access to the 'Editor' instance.
     * @return The 'Editor' instance.
     */
    Editor *getEditor();

    /**
     * @brief Provides access to the 'MenuBar' instance.
     * @return The 'MenuBar' instance.
     */
    MenuBar *getMenuBar();

    /**
     * @brief Provides access to the 'StatusBar' instance.
     * @return The 'StatusBar' instance.
     */
    StatusBar *getStatusBar();

    /**
     * @brief Prompts the user to open file(s).
     */
    void open();

    /**
     * @brief Saves the current file.
     * @note If no file is opened,
     * opens a file dialog to select the save location.
     * @return Whether the file is saved successfully.
     */
    bool save();

    /**
     * @brief Opens a file dialog to select the save location.
     */
    void saveAs();

    /**
     * @brief Opens a font dialog for selecting a new editor font.
     */
    void selectNewFont();

    /**
     * @brief Opens a new window.
     */
    static void newWindow();

    /**
     * @brief Opens a file in a new window.
     * @param path The file path.
     */
    static void open(const QString &path);

    /**
     * @brief Adds a recent file path.
     * @param path The path of a recently opened file.
     */
    static void addRecent(const QString &path);

    /**
     * @brief Clears all recent file paths.
     */
    static void clearRecent();

    /**
     * @brief Shows or hides the line numbers on the left side of the editor.
     * @param shown Whether the line numbers are shown.
     */
    static void showLineNum(bool shown);

    /**
     * @brief Shows or hides the status bar on the bottom.
     * @param shown Whether the status bar is shown.
     */
    static void showStatus(bool shown);

    /**
     * @brief Sets the word wrap mode of the editor.
     * @param wrap Whether to wrap word in the editor.
     */
    static void setWordWrap(bool wrap);

    /**
     * @brief Increases the editor font size by 10%.
     */
    static void zoomIn();

    /**
     * @brief Decreases the editor font size by 10%.
     * @note The editor font size cannot be lower than 1pt.
     */
    static void zoomOut();

    /**
     * @brief Sets the editor font size back to the default (100%).
     */
    static void resetZoom();

protected:
    void closeEvent(QCloseEvent *event);

    // Enable drag & drop of files
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    Editor *editor;
    MenuBar *menuBar;
    StatusBar *statusBar;

    // The file path
    QString filePath;
    // The file name
    QString fileName;
    // Whether the file is saved
    bool saved;

    // Store all 'MainWindow' instances
    static QList<MainWindow *> windows;
    // Extension filter of the file dialog
    static const QString EXT_FILTER;

    /**
     * @brief Initializes a new 'MainWindow' instance.
     * @param path The file path.
     */
    MainWindow(const QString &path = "");
    ~MainWindow();

    /**
     * @brief Saves the file to the specified location.
     * @param The file path.
     */
    void save(const QString &path);

    /**
     * @brief Updates the save state when modifying the file.
     */
    void updateSave();

    /**
     * @brief Shows or hides the asterisk symbol (*) on the title bar.
     */
    void updateTitle();

    /**
     * @brief Sets the zoom percentage of the editor font size.
     * @param zoom The zoom percentage of the editor font size.
     */
    static void setZoom(int zoom);
};
