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
    QMAKE_CXXFLAGS_DEBUG += -O0
    QMAKE_CXXFLAGS_RELEASE += -O2
    LIBS += -luserenv -LC:/OpenSSL-Win32 -leay32 -lssl32
}

HEADERS += constant.h \
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

RC_FILE += resource.rc
