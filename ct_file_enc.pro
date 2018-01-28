#-------------------------------------------------
#
# CT File Encrypt
#
# Author: Carlos Tse
# Email: carlos@aboutmy.info
# Date: JAN 29, 2018
#-------------------------------------------------

QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = ct_file_encrypt
TEMPLATE = app
OPENSSL = /usr/local/opt/openssl@1.1

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

macx {
    INCLUDEPATH += $${OPENSSL}/include
    QMAKE_LIBDIR += $${OPENSSL}/lib
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
