#include "WinAPI.h"
#include <windows.h>
#include <QTimer>
#include <QApplication>
#include <QWidget>
#include <QWindow>

bool dark;

bool WinAPI::isDarkModeEnabled() {
    HKEY hKey;
    DWORD regValue = 0;
    DWORD regValueSize = sizeof(regValue);
    const wchar_t* regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";

    if (RegOpenKeyExW(HKEY_CURRENT_USER, regPath, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExW(hKey, L"AppsUseLightTheme", nullptr, nullptr, 
                             reinterpret_cast<LPBYTE>(&regValue), &regValueSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return regValue == 0;
        }
        RegCloseKey(hKey);
    }

    return false;
}

void WinAPI::detectModeChange() {
    dark = isDarkModeEnabled();

    auto timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [] {
        bool newDark = isDarkModeEnabled();
        if (newDark != dark) {
            QApplication::setStyle("Fusion");
            dark = newDark;
        }
    });
    timer->start(1000);
}

void WinAPI::detectDpiChange() {
    auto timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [] {
        for (auto &widget : QApplication::allWidgets()) {
            widget->update();

            if (widget->windowFlags() & Qt::MSWindowsFixedSizeDialogHint) {
                widget->adjustSize();
            }
        }
    });
    timer->start(1000);
}
