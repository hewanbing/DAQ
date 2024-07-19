QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    myData.cpp \
    myString.cpp \
    v2718.cpp \
    vhs.cpp \
    vmehandle.cpp \
    vmusb.cpp

HEADERS += \
    IEEE754.h \
    mainwindow.h \
    myData.h \
    myString.h \
    v2718.h \
    vhs.h \
    vmehandle.h \
    vmusb.h

INCLUDEPATH += /opt/root-6.22.0/include

LIBS += -L/opt/root-6.22.0/lib \
    -lCore \
    -lImt \
    -lRIO \
    -lNet \
    -lHist \
    -lGraf \
    -lGraf3d \
    -lGpad \
    -lROOTVecOps \
    -lTree \
    -lTreePlayer \
    -lRint \
    -lPostscript \
    -lMatrix \
    -lPhysics \
    -lMathCore \
    -lThread \
    -lMultiProc \
    -lROOTDataFrame \
    -pthread \
    -lm \
    -ldl \
    -rdynamic \
    -lCAENVME \
    -lboost_system \
    -lxx_usb \
    -lusb \
    -lGui \
    -lEve \
    -lHistPainter \
    -lRIO \
    -lRHTTP


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
