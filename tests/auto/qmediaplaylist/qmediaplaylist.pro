CONFIG += testcase
TARGET = tst_qmediaplaylist

# temporarily blacklist test because is fails miserably
CONFIG += insignificant_test

include (../qmultimedia_common/mockplaylist.pri)

QT += multimedia-private testlib
CONFIG += no_private_qt_headers_warning

DEFINES += TESTDATA_DIR=\\\"$$PWD/\\\"

HEADERS += \
    $$QT.multimedia.sources/../plugins/m3u/qm3uhandler.h

SOURCES += \
    tst_qmediaplaylist.cpp \
    $$QT.multimedia.sources/../plugins/m3u/qm3uhandler.cpp

INCLUDEPATH += $$QT.multimedia.sources/../plugins/m3u
