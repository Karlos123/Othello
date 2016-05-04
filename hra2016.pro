QT += core gui widgets

CONFIG += c++11

TARGET = hra2016
CONFIG -= console app_bundle
RESOURCES = hra2016.qrc
DEFINES += OTH_USE_GUI

TEMPLATE = app

SOURCES += main.cpp \
    board.cpp \
    game.cpp \
    game-logic.cpp \
    history.cpp \
    ai1.cpp \
    guiwindow.cpp \
    guiboardarea.cpp \
    ai2.cpp

HEADERS += \
    board.hpp \
    game.hpp \
    game-logic.hpp \
    history.hpp \
    types.hpp \
    ai1.hpp \
    guiboardarea.hpp \
    guiwindow.hpp \
    ai2.hpp
