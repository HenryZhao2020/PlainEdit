#pragma once

/**
 * @brief Contains the names of available languages.
 */
namespace Lang {
    extern const QString ENGLISH;
    extern const QString CHINESE_SIMPLIFIED;
}

/**
 * @brief Contains shared attributes for saving and loading program state.
 */
class Attr {
public:
    // Recent files
    /**
     * @brief Directory that the most recent file is opened or saved to.
     */
    QString recentDir = QDir::homePath();
    /**
     * @brief Paths of recently opened files.
     */
    QStringList recentPaths;

    // Find & Replace
    /**
     * @brief Text snippet to be searched.
     */
    QString findTarget;
    /**
     * @brief Text snippet to be replaced.
     */
    QString replaceTarget;
    /**
     * @brief Whether to match case upon searching.
     */
    bool matchCase = false;
    /**
     * @brief Whether to match whole word upon searching.
     */
    bool matchWholeWord = false;

    // View options
    /**
     * @brief Whether to show line numbers on the left side of the editor.
     */
    bool showLine = true;
    /**
     * @brief Whether to show the status bar on the bottom.
     */
    bool showStatus = true;
    /**
     * @brief Whether to wrap words when the editor resizes.
     */
    bool wordWrap = false;
    /**
     * @brief Zoom percentage of the editor font size.
     */
    int zoom = 100;

    // Other preferences
    /**
     * @brief Editor font.
     */
    QFont editorFont = QFont({"Cascadia Code", "Microsoft Yahei UI"}, 12);
    /**
     * @brief Display language.
     */
    QString lang = Lang::ENGLISH;

    // Save & Load
    /**
     * @brief Saves all attributes to the program folder.
     */
    void save();
    /**
     * @brief Loads all attributes from the program folder.
     * @return true if successful; false otherwise.
     */
    bool load();

    // Singleton access
    /**
     * @brief Provides access to the singleton instance.
     * @return The singleton instance.
     */
    static Attr &get();

private:
    // Private constructor to prevent external instantiation
    Attr();
    ~Attr();
};
