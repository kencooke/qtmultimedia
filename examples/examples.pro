TEMPLATE = subdirs

SUBDIRS += \
    radio \
    camera \
    slideshow \
    audiorecorder \
    audiodevices \
    audioinput \
    audiooutput \
    videographicsitem \
    videowidget \
    player \

contains(QT_CONFIG, declarative) {
    SUBDIRS += declarative-camera
}

