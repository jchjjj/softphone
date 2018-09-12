#-------------------------------------------------
#
# Project created by QtCreator 2018-08-02T14:04:28
#
#-------------------------------------------------

QT       += core gui multimediawidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = softPhone
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    settingsdialog.cpp \
    videodialog.cpp \
    chatwindow.cpp \
    numberkey.cpp \
    database.cpp \
    sip/Manager.cpp \
    sip/sipAccount.cpp \
    sip/sipBuddy.cpp \
    sip/sipCall.cpp \
    sip/UI.cpp \
    sip/uniSip.cpp \
    sipthread.cpp \
    global.cpp \
    model/message.cpp \
    model/config.cpp


HEADERS += \
        mainwindow.h \
    chatwindow.h \
    settingsdialog.h \
    videodialog.h \
    numberkey.h \
    database.h \
    sip/head_all.hpp \
    sip/Manager.hpp \
    sip/sipAccount.hpp \
    sip/sipBuddy.hpp \
    sip/sipCall.hpp \
    sip/UI.hpp \
    sipthread.h \
    global.h \
    model/message.h \
    model/config.h \
    util.h


FORMS += \
        mainwindow.ui \
    chatwindow.ui \
    settingsdialog.ui \
    videodialog.ui

RESOURCES += \
    icon.qrc

INCLUDEPATH += /usr/local/include
LIBS += -g -O2 -DPJ_AUTOCONF=1 -DPJ_IS_BIG_ENDIAN=0 -DPJ_IS_LITTLE_ENDIAN=1   -L/usr/local/lib \
    -lpjsua2-x86_64-unknown-linux-gnu -lstdc++ -lpjsua-x86_64-unknown-linux-gnu \
    -lpjsip-ua-x86_64-unknown-linux-gnu -lpjsip-simple-x86_64-unknown-linux-gnu  \
    -lpjsip-x86_64-unknown-linux-gnu -lpjmedia-codec-x86_64-unknown-linux-gnu \
    -lpjmedia-videodev-x86_64-unknown-linux-gnu -lpjmedia-audiodev-x86_64-unknown-linux-gnu \
    -lpjmedia-x86_64-unknown-linux-gnu -lpjnath-x86_64-unknown-linux-gnu \
    -lpjlib-util-x86_64-unknown-linux-gnu -lsrtp-x86_64-unknown-linux-gnu \
    -lresample-x86_64-unknown-linux-gnu -lgsmcodec-x86_64-unknown-linux-gnu \
    -lspeex-x86_64-unknown-linux-gnu -lilbccodec-x86_64-unknown-linux-gnu \
    -lg7221codec-x86_64-unknown-linux-gnu -lyuv-x86_64-unknown-linux-gnu \
    -lwebrtc-x86_64-unknown-linux-gnu -lpj-x86_64-unknown-linux-gnu -lopus -lssl \
    -lcrypto -luuid -lm -lrt -lpthread -lasound -lavdevice -lavformat -lavcodec \
    -lswscale -lavutil -lv4l2 -lSKP_SILK_SDK

DISTFILES += \
    sounds/busytone.wav \
    sounds/dialtone.wav \
    sounds/newmessage.wav \
    sounds/ring.wav \
    sounds/voicemail.wav
