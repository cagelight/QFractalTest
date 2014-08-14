CONFIG += c++11
CONFIG += Debug

QT += core gui
QT += widgets

TARGET = QFractalTest
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/executive.cpp \
    src/ui.cpp \
    src/render.cpp \
    src/qwidget_gradient.cpp \
    src/common.cpp \
    src/qwidget_2dnavigator.cpp \
    src/ui_main.cpp \
    src/ui_render.cpp \
    src/render_pixel.c \
    src/multigradient.cpp

HEADERS  += \
    src/executive.hpp \
    src/ui.hpp \
    src/render.hpp \
    src/debug.hpp \
    src/qwidget_gradient.hpp \
    src/common.hpp \
    src/qwidget_2dnavigator.hpp \
    src/ui_main.hpp \
    src/ui_render.hpp \
    src/ui_delegator.hpp \
    src/render_pixel.h \ 
    src/multigradient.hpp \
    src/fractal_settings.h \
    src/fractal_meta.hpp

Release:DESTDIR = release
Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:DESTDIR = debug
Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui
