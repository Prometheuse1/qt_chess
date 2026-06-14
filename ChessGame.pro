QT       += core gui widgets

CONFIG   += c++17

TARGET   = ChessGame
TEMPLATE = app

SOURCES += \
    main_qt.cpp \
    chessboard.cpp \
    plateau.cpp

HEADERS += \
    chessboard.h \
    plateau.h
