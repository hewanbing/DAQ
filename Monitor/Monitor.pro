TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        dict.cpp \
        Event.cpp \
        EventBase.cpp \
        main.cpp \
        monitor.cpp

HEADERS += \
    Channel.h \
    Event.h \
    EventBase.h \
    monitor.h

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

DISTFILES += \
    Monitor.pro.user \
    dict_rdict.pcm
