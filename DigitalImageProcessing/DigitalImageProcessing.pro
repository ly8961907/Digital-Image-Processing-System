#-------------------------------------------------
#
# Project created by QtCreator 2019-10-01T18:24:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = DigitalImageProcessing
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
        bitplane.cpp \
        main.cpp \
        mainwindow.cpp \
        qcustomplot.cpp

HEADERS += \
        bitplane.h \
        mainwindow.h \
        qcustomplot.h

FORMS += \
        bitplane.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += E:\Opencv\eigen\Eigen
INCLUDEPATH += E:\Opencv\OpenCV-MinGW-Build-OpenCV-4.1.1-x64\include
LIBS += E:\Opencv\OpenCV-MinGW-Build-OpenCV-4.1.1-x64\x64\mingw\bin\libopencv_*.dll
