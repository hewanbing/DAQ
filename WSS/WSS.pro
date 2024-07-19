TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        MString.cpp \
        main.cpp \
        wss.cpp

HEADERS += \
    MString.h \
    wss.h

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
    -lRHTTP \
    -ljsoncpp \
    -lrt

DISTFILES +=
