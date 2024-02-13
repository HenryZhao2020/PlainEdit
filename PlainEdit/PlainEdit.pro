QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Attr.cpp \
    Dialog.cpp \
    Editor.cpp \
    File.cpp \
    Icon.cpp \
    Main.cpp \
    MainWindow.cpp \
    MenuBar.cpp \
    StatusBar.cpp \
    WinGui.cpp

HEADERS += \
    Attr.h \
    Dialog.h \
    Editor.h \
    File.h \
    Icon.h \
    MainWindow.h \
    MenuBar.h \
    PCH.h \
    StatusBar.h \
    WinGui.h

PRECOMPILED_HEADER += PCH.h

include(singleapplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

TARGET = "PlainEdit"

QMAKE_TARGET_DESCRIPTION = "PlainEdit"

RC_ICONS = "Logo.ico"

VERSION = 2.0.0

TRANSLATIONS += \
    PlainEdit_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
