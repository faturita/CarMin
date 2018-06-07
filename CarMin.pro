#-------------------------------------------------
#
# Project created by QtCreator 2015-03-17T19:18:30
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = CarMin
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    help.cpp

INCLUDEPATH += D:\opencv\build\include\

LIBS += -LD:\opencv\build\x86\vc11\lib \
-lopencv_core249d \
-lopencv_highgui249d \
-lopencv_imgproc249d \
-lopencv_features2d249d \
-lopencv_calib3d249d \
-lopencv_nonfree249d
