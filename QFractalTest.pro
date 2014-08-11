#-------------------------------------------------
#
# Project created by QtCreator 2014-07-25T18:40:10
#
#-------------------------------------------------

CONFIG += c++11

QT += core gui
QT += widgets

TARGET = QFractalTest
TEMPLATE = app


SOURCES += main.cpp \
    executive.cpp \
    ui.cpp \
    render.cpp \
    fractal.cpp \
    qwidget_gradient.cpp \
    common.cpp \
    qwidget_2dnavigator.cpp \
    ui_main.cpp \
    ui_render.cpp \
    render_pixel.c

HEADERS  += \
    executive.hpp \
    ui.hpp \
    render.hpp \
    fractal.hpp \
    debug.hpp \
    qwidget_gradient.hpp \
    common.hpp \
    qwidget_2dnavigator.hpp \
    ui_main.hpp \
    ui_render.hpp \
    ui_delegator.hpp \
    render_pixel.h
