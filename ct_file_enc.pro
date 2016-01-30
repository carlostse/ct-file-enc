#-------------------------------------------------
#
# CT File Encrypt
#
# Author: Carlos Tse
# Email: copperoxide@gmail.com
# Date: 28 JAN 2016
#-------------------------------------------------

QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = ct_file_encrypt
TEMPLATE = app

CONFIG(debug, debug|release) {
    message(Debug build)
    DESTDIR = debug
} else {
    message(Release build)
    DESTDIR = release
}
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

QMAKE_CXXFLAGS += -std=gnu++11 -Wno-unknown-pragmas -Wno-switch -Wno-unused-result
QMAKE_CXXFLAGS_DEBUG += -O0 -Wall -g
QMAKE_CXXFLAGS_RELEASE += -O2

win32 {
    INCLUDEPATH += C:/OpenSSL-Win32/include
}

win32-g++ {
    DEFINES -= UNICODE
    QMAKE_LIBDIR += C:/OpenSSL-Win32
    LIBS += -luserenv -leay32 -lssl32
}

macx {
    INCLUDEPATH += /usr/local/Cellar/openssl/1.0.2e_1/include
    QMAKE_LIBDIR += /usr/local/opt/openssl/lib
    QMAKE_CXXFLAGS += -Winconsistent-missing-override
    LIBS += -lcrypto -lssl
    ICON = icon.icns
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
