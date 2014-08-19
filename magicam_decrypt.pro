#-------------------------------------------------
#
#
#
#-------------------------------------------------

QT += core gui widgets
TARGET = magicam_decrypt
TEMPLATE = app

QMAKE_CXXFLAGS += -Wunused-parameter

win32 {
    INCLUDEPATH += C:/OpenSSL-Win32/include
    LIBS += -LC:/OpenSSL-Win32 -leay32 -lssl32
}

HEADERS += \
    byte_array.h \
    util.h \
    key_util.h \
    drop_area.h \
    drop_window.h

SOURCES += main.cpp\
    byte_array.cpp \
    util.cpp \
    key_util.cpp \
    drop_area.cpp \
    drop_window.cpp
