QT += core

CONFIG += c++11

TARGET = hra2016-cli
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    board.cpp \
    game.cpp \
    game-logic.cpp \
    history.cpp \
    ai1.cpp \
    ai2.cpp

HEADERS += \
    board.hpp \
    game.hpp \
    game-logic.hpp \
    history.hpp \
    types.hpp \
    ai1.hpp \
    ai2.hpp
