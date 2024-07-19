TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        MString.cpp \
        MTimer.cpp \
        dict.cpp \
        dpc.cpp \
        Event.cpp \
        EventBase.cpp \
        main.cpp

HEADERS += \
    BufferTP.h \
    Channel.h \
    MString.h \
    MTimer.h \
    dpc.h \
    Event.h \
    EventBase.h

DISTFILES += \
    DPC.pro.user \
    dict_rdict.pcm

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
