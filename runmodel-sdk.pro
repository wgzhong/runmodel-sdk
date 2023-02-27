#-------------------------------------------------
#
# Project created by QtCreator 2022-08-21T20:06:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = runmodel-sdk
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
    camera/camera.cpp \
    sdk/runmodel.cpp \
    sdk/utils.cpp \
    test/yolov5s/yolov5s.cpp

HEADERS += \
        mainwindow.h \
    camera/camera.h \
    protocol/uart.h \
    sdk/runmodel.h \
    sdk/utils.h \
    test/yolov5s/test_yolov5s.h \
    test/yolov5s/yolov5s.h \
    time/cal_time.h \
    tools/tool.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += /home/wgzhong/tvm/include
INCLUDEPATH += /home/wgzhong/tvm/3rdparty/dmlc-core/include
INCLUDEPATH += /home/wgzhong/tvm/3rdparty/dlpack/include
LIBS += /home/wgzhong/tvm/build/libtvm.so

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
LIBS += /usr/local/lib/libopencv_core.so \
        /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_imgcodecs.so \
        /usr/local/lib/libopencv_videoio.so \
        /usr/local/lib/libopencv_video.so
