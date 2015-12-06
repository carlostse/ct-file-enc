#-------------------------------------------------
#
# CT File Encrypt
#
# Author: Carlos Tse
# Email: copperoxide@gmail.com
# Date: 18 AUG 2014
#-------------------------------------------------

QT += core gui widgets
TARGET = ct_file_enc
TEMPLATE = app

win32 {
    INCLUDEPATH += C:/OpenSSL-Win32/include
}

win32-g++ {
    DEFINES -= UNICODE
    QMAKE_CXXFLAGS += -std=gnu++11 -Wno-unknown-pragmas -Wno-switch -Wno-unused-result
    QMAKE_CXXFLAGS_DEBUG += -O0 -Wall -g
    QMAKE_CXXFLAGS_RELEASE += -O2
    QMAKE_LIBDIR += C:/OpenSSL-Win32
    LIBS += -luserenv -leay32 -lssl32
}

HEADERS += constant.h \
    byte_array.h \
    util.h \
    master_key.h \
    drop_area.h \
    clickable_label.h \
    drop_window.h

SOURCES += main.cpp\
    byte_array.cpp \
    util.cpp \
    master_key.cpp \
    drop_area.cpp \
    clickable_label.cpp \
    drop_window.cpp

RC_FILE += resource.rc
