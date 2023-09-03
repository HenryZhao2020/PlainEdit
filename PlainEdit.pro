QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = "PlainEdit"

QMAKE_TARGET_DESCRIPTION = "PlainEdit"

RC_ICONS = "Logo.ico"

SOURCES += \
    Attr.cpp \
    Dialog.cpp \
    Editor.cpp \
    File.cpp \
    Main.cpp \
    MainWindow.cpp \
    MenuBar.cpp \
    Pixmap.cpp

HEADERS += \
    Attr.h \
    Dialog.h \
    Editor.h \
    File.h \
    MainWindow.h \
    MenuBar.h \
    PCH.h \
    Pixmap.h

PRECOMPILED_HEADER = PCH.h

include(SingleApplication/SingleApplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

LIBS += -luser32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
