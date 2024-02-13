#include "WinGui.h"

void WinGui::raiseWidget(QWidget *widget) {
    HWND hwnd = (HWND) widget->winId();

    // Check if widget is minimized to Windows task bar
    if (IsIconic(hwnd)) {
        ShowWindow(hwnd, SW_RESTORE);
    }

    SetForegroundWindow(hwnd);
}

bool WinGui::isDarkMode() {
    static QSettings entry("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\"
                           "CurrentVersion\\Themes\\Personalize",
                           QSettings::NativeFormat);
    return entry.value("AppsUseLightTheme") != 1;
}

void WinGui::relaunch() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
