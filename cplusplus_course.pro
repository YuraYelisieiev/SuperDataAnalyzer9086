TEMPLATE = app
QT = core gui charts serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

SOURCES += main.cpp \
    settingsdialog.cpp \
    mainwindow.cpp \
    chartwindow.cpp

RESOURCES += \
    new_qrc.qrc

HEADERS += \
    settingsdialog.h \
    mainwindow.h \
    chartwindow.h

DISTFILES +=

FORMS += \
    settingsdialog.ui \
    mainwindow.ui
