TEMPLATE = app
QT = core gui charts serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

SOURCES += main.cpp

RESOURCES += \
    new_qrc.qrc
