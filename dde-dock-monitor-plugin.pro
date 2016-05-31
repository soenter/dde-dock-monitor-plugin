include(../../common.pri)

QT       += core gui widgets

TARGET = dde-dock-monitor-plugin
TEMPLATE = lib
CONFIG += plugin c++11

SOURCES += \ 
    watch.cpp \
    monitorplugin.cpp

HEADERS += \ 
    watch.h \
    monitorplugin.h
INCLUDEPATH += ../dde-dock/src/

DISTFILES += dde-dock-monitor-plugin.json

target.path = $${PREFIX}/lib/dde-dock/plugins/
INSTALLS += target

QMAKE_MOC_OPTIONS += -I/usr/include/

RESOURCES += \
    res.qrc

FORMS += \
    watch.ui
