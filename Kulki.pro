#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T18:30:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qoolkie
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    gamemap.cpp \
    game.cpp \
    highscore.cpp

HEADERS  += mainwindow.h \
    gamemap.h \
    game.h \
    timer.h \
    highscore.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
