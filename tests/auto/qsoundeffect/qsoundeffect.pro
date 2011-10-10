TARGET = tst_qsoundeffect

QT += core declarative multimedia-private testlib
CONFIG += no_private_qt_headers_warning

# This is more of a system test
# CONFIG += testcase

SOURCES += tst_qsoundeffect.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix:!mac {
    !contains(QT_CONFIG, pulseaudio) {
        DEFINES += QT_MULTIMEDIA_QMEDIAPLAYER
    }
}
