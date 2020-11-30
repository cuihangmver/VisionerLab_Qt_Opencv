#-------------------------------------------------
#
# Project created by QtCreator 2020-10-03T10:48:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImLab
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





CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    form.cpp \
    inforbase.cpp \
    centralwidget.cpp \
    dialogslider.cpp \
    adaptivedialog.cpp \
    gaussiandialog.cpp \
    laplace.cpp \
    connectedregion.cpp

# opencv3.4.0
INCLUDEPATH += E:/ProfessionalSoftware/QT/opencv/opencv/build/include \
               E:/ProfessionalSoftware/QT/opencv/opencv/build/include/opencv \
               E:/ProfessionalSoftware/QT/opencv/opencv/build/include/opencv2
LIBS+=E:/ProfessionalSoftware/QT/opencv/opencv/build/x64/vc14/lib/opencv_world340d.lib
LIBS+=E:/ProfessionalSoftware/QT/opencv/opencv/build/x64/vc14/lib/opencv_world340.lib
HEADERS += \
        mainwindow.h \
    form.h \
    inforbase.h \
    centralwidget.h \
    dialogslider.h \
    adaptivedialog.h \
    gaussiandialog.h \
    laplace.h \
    connectedregion.h

FORMS += \
        mainwindow.ui \
    form.ui \
    centralwidget.ui \
    dialogslider.ui \
    adaptivedialog.ui \
    gaussiandialog.ui \
    laplace.ui \
    connectedregion.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    VisionerLab.rc
RC_FILE = \
    VisionerLab.rc
