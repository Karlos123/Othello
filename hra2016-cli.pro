QT += core gui widgets

CONFIG += c++11

TARGET = hra2016-cli
CONFIG += console
CONFIG -= app_bundle
RESOURCES = hra2016-cli.qrc

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
