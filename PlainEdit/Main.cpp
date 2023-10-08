#include "MainWindow.h"
#include "Attr.h"

void receivedMessage(quint32, QByteArray message) {
    if (message == "New") {
        MainWindow::newWindow();
    } else if (message.startsWith("Open")) {
        MainWindow::open(message.mid(5));
    }
}

int main(int argc, char *argv[]) {
    SingleApplication app(argc, argv, true);

    if (app.isSecondary()) {
        AllowSetForegroundWindow(DWORD(app.primaryPid()));
        if (argc == 1) {
            app.sendMessage("New");
        } else {
            for (int i = 1; i < argc; i++) {
                app.sendMessage("Open " + QByteArray(argv[i]));
            }
        }
        return 0;
    }

    QDir::setCurrent(QApplication::applicationDirPath());    
    QDir::setCurrent("Res");

    if (!Attr::load()) {
        Attr::fontFamily = QApplication::font().family();
        Attr::fontWeight = QApplication::font().weight();
    }

    app.setStyle("Fusion");
    app.setStyleSheet(File::readAll("Styles.qss"));
    QObject::connect(&app, &SingleApplication::aboutToQuit, &app, &Attr::save);
    QObject::connect(&app, &SingleApplication::receivedMessage,
                     &app, &receivedMessage);

    if (argc == 1) {
        MainWindow::newWindow();
    } else {
        for (int i = 1; i < argc; i++) {
            MainWindow::open(argv[i]);
        }
    }

    return app.exec();
}
