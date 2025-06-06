QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/MyModbus/MyModbus.pri)

#设置字符(MSCV编译器下防止中文乱码)
contains( CONFIG,"msvc" ):QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
contains( CONFIG,"msvc" ):QMAKE_CFLAGS +=/source-charset:utf-8 /execution-charset:utf-8


SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

