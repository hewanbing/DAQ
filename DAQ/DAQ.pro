TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        MData.cpp \
        MString.cpp \
        VMUSB.cpp \
        daq.cpp \
        dict.cpp \
        main.cpp \
        v1290.cpp \
        v2718.cpp \
        v785.cpp


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

HEADERS += \
    BufferTP.h \
    Channel.h \
    DAQHandle.h \
    FEE.h \
    FEEIRQ.h \
    MData.h \
    MString.h \
    VMUSB.h \
    bufferTP.h \
    channel.h \
    daq.h \
    v1290.h \
    v2718.h \
    v785.h

DISTFILES += \
    DAQ.pro.user \
    dict_rdict.pcm
