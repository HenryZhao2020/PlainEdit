#include "MainWindow.h"
#include "Attr.h"

#include "SingleApplication"

// Whether the OS is in dark mode
bool dark = WinGui::isDarkMode();

/**
 * @brief Processes communication between different program instances.
 * @param message Sent message to the primary instance.
 */
void processMessage(quint32, QByteArray message) {
    if (message == "New") {
        MainWindow::newWindow();
    } else if (message.startsWith("Open")) {
        MainWindow::open(message.mid(5));
    }
}

/**
 * @brief Detects and adapts to any OS theme change.
 */
void adaptTheme() {
    bool newDark = WinGui::isDarkMode();
    if (newDark != dark) {
        QApplication::setStyle("Fusion");
        dark = newDark;
    }
}

/**
 * @brief Sets up the program, then starts the game.
 * @param argc Number of arguments.
 * @param argv List of arguments.
 * @return Execution code.
 */
int main(int argc, char **argv) {
    SingleApplication app(argc, argv, true);
    AllowSetForegroundWindow(DWORD(app.primaryPid()));

    // Read the arguments
    if (app.isSecondary()) {
        // If no arguments are given, open a new window
        if (argc == 1) {
            app.sendMessage("New");
        // Otherwise, open the files given by the arguments
        } else {
            for (int i = 1; i < argc; ++i) {
                app.sendMessage("Open " + QByteArray(argv[i]));
            }
        }
        // Exit the secondary instance
        return 0;
    }

    // Set current directory to the parent folder
    QDir::setCurrent(app.applicationDirPath());
    // Set current directory to 'Res'
    QDir::setCurrent("Res");

    // Load saved attributes if possible
    Attr::get().load();

    // Update the display language
    QTranslator baseTrans, appTrans;
    if (Attr::get().lang == Lang::CHINESE_SIMPLIFIED) {
        if (baseTrans.load("qtbase_zh_CN", QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
            app.installTranslator(&baseTrans);
        }

        if (appTrans.load("../PlainEdit_zh_CN")) {
            app.installTranslator(&appTrans);
        }
    }

    // Load external fonts
    // For displaying chinese characters
    QFontDatabase::addApplicationFont("MicrosoftYaheiUI-Regular.ttc");
    // For displaying monospaced characters
    QFontDatabase::addApplicationFont("CascadiaMono.ttf");
    QFontDatabase::addApplicationFont("CascadiaCode.ttf");
    
    // Set application name
    app.setApplicationName(QApplication::tr("PlainEdit"));

    // Style application
    app.setStyle("Fusion");
    app.setStyleSheet(File::readAll("Styles.qss"));

    // Save attributes on quit
    QObject::connect(&app, &QApplication::aboutToQuit, &app, [] {
        Attr::get().save();
    });

    // Process communication between different program instances
    QObject::connect(&app, &SingleApplication::receivedMessage,
                     &app, &processMessage);

    // Detect theme change every second
    QTimer themeTimer;
    QObject::connect(&themeTimer, &QTimer::timeout, &app, &adaptTheme);
    themeTimer.start(1000);

    // If no arguments are given, open a new window
    if (argc == 1) {
        MainWindow::newWindow();
    // Otherwise, open the files given by the arguments
    } else {
        for (int i = 1; i < argc; ++i) {
            MainWindow::open(argv[i]);
        }
    }

    return app.exec();
}
