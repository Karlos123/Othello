QT += core
QT -= gui

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
    main-game.cpp

HEADERS += \
    board.hpp \
    game.hpp \
    game-logic.hpp \
    history.hpp \
    types.hpp \
    main-game.hpp
