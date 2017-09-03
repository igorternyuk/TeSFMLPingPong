TEMPLATE = app
CONFIG += c++1z
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += DEBUG
LIBS += `pkg-config --libs sfml-all`
SOURCES += main.cpp \
    game.cpp \
    mainmenustate.cpp \
    playstate.cpp \
    ball.cpp \
    paddle.cpp

HEADERS += \
    game.h \
    mainmenustate.h \
    playstate.h \
    screenstate.h \
    ball.h \
    paddle.h \
    consts.h

