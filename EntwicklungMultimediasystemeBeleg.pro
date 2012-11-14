#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T13:21:24
#
#-------------------------------------------------

QT       += core gui

TARGET = EntwicklungMultimediasystemeBeleg
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mygraphicsview.cpp \
    mysliderdialog.cpp

HEADERS  += mainwindow.h \
    mygraphicsview.h \
    mysliderdialog.h

FORMS    += mainwindow.ui \
    sliderdialog.ui

TRANSLATIONS = Translation_de.ts

RESOURCES += \
    rescource.qrc