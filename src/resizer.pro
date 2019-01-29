include(../images/images.pri)
include(widgets/widgets.pri)

QT += core gui widgets

CONFIG += c++1z

win32 {
    RC_FILE = ../icon.rc
    QT += winextras
}

TARGET = resizer
TEMPLATE = app
TRANSLATIONS += resizer_en.ts

HEADERS += \
    filters.h \
    dialog.h \
    util.h \
    finisheddialog.h \
    global.h \
    resizeworker.h \
    dialog_ui.h

SOURCES += \
    main.cpp \
    dialog.cpp \
    util.cpp \
	finisheddialog.cpp \
    resizeworker.cpp \
    filters.cpp
