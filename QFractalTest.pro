CONFIG += c++11
CONFIG += Debug

QT += core gui
QT += widgets

TARGET = QFractalTest
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/ui.cpp \
    src/qwidget_gradient.cpp \
    src/qwidget_2dnavigator.cpp \
    src/ui_main.cpp \
    src/ui_render.cpp \
    src/render_pixel.c \
    src/multigradient.cpp \
    src/qwidget_renderpreview.cpp \
    src/qobject_fractalrenderer.cpp \
    src/fractal_settings.c

HEADERS  += \
    src/ui.hpp \
    src/debug.hpp \
    src/qwidget_gradient.hpp \
    src/common.hpp \
    src/qwidget_2dnavigator.hpp \
    src/ui_main.hpp \
    src/ui_render.hpp \
    src/render_pixel.h \ 
    src/multigradient.hpp \
    src/fractal_settings.h \
    src/qwidget_renderpreview.hpp \
    src/qobject_fractalrenderer.hpp \
    src/render_enumerations.h \
    src/qfractalmeta.hpp

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
